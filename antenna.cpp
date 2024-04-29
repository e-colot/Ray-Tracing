#include "antenna.h"
#include "math.h"
#include <iostream>

// ---------- CONSTRUCTORS ----------

Antenna::Antenna() {}
RealAntenna::RealAntenna() {}
RealAntenna::RealAntenna(const Vector& position) : emission_factor(Complex(0.0f)), min_attenuation(0.0f), max_attenuation(0.0f) {
    src = this;
    wall = nullptr;
    pos = position;
}

VirtualAntenna::VirtualAntenna() {}
VirtualAntenna::VirtualAntenna(RealAntenna* realAntenna, Wall* reflectedWall) {
    src = realAntenna;
    wall = reflectedWall;
    pos = wall->mirror(src->get_pos());
}

// ---------- DESTRUCTORS ----------

RealAntenna::~RealAntenna() {
    for (int i = 0; i < static_cast<int>(virtual_network.size()); i++) {
        delete virtual_network[i];
    }
    for (int i = 0; i < static_cast<int>(rays.size()); i++) {
        delete rays[i];
    }
}

// ---------- ACCESSORS ----------

Vector Antenna::get_pos() const {
    return pos;
}
RealAntenna* Antenna::get_src() const {
    return src;
}
Wall* Antenna::get_wall() const {
    return wall;
}

antennaVect RealAntenna::get_virtual_network() const {
    return virtual_network;
}
rayVect RealAntenna::get_rays() const {
    return rays;
}

float RealAntenna::get_min_attenuation() const
{
    return min_attenuation;
}

float RealAntenna::get_max_attenuation() const
{
    return max_attenuation;
}

// ---------- MUTATORS ----------

void RealAntenna::set_min_attenuation(float a)
{
    min_attenuation = a;
}
void RealAntenna::set_max_attenuation(float a)
{
    max_attenuation = a;
}

// ---------- METHODS ----------

void Antenna::create_ray(Antenna* rx, const wallVect& walls) {
    if ((get_wall() != rx->get_wall()) or ((get_wall() == nullptr) && (rx->get_wall() == nullptr))) {
        // making sure it's not two virtual antennas mirrored on the same wall
        // the condition after the or is true if the two antennas are RealAntennas -> direct link
        Ray* new_ray_ptr = nullptr;
        // using new so the location of the pointer don't get deleted out of this stack
        Ray* new_ray = new Ray(this, rx, &new_ray_ptr, walls);
        // implies that it needs to be deleted when we don't use it anymore
        if (new_ray_ptr != nullptr) {
            add_ray(new_ray);
            if (PHASE) {
                if (src->get_max_attenuation() < new_ray->get_attenuation().squared_norm()) {
                    src->set_max_attenuation(new_ray->get_attenuation().squared_norm());
                }
                else if (src->get_min_attenuation() > new_ray->get_attenuation().squared_norm()) {
                    src->set_min_attenuation(new_ray->get_attenuation().squared_norm());
                }
            }
            else{
                if (src->get_max_attenuation() < new_ray->get_attenuation().get_real()) {
                    src->set_max_attenuation(new_ray->get_attenuation().get_real());
                }
                else if (src->get_min_attenuation() > new_ray->get_attenuation().get_real()) {
                    src->set_min_attenuation(new_ray->get_attenuation().get_real());
                }
            }
        }
        new_ray_ptr = nullptr;
    }
}

void RealAntenna::virtualize(Wall* w) {
    virtual_network.push_back(new VirtualAntenna(this, w));
}
void RealAntenna::add_ray(Ray* r) {
    rays.push_back(r);
    emission_factor += r->get_attenuation();
}
float RealAntenna::get_binary_rate() const {
    // 4.5 from exercises syllabus
    // + 5.27 from syllabus
    // + 5.49 from syllabus with theta = 0
    // + 1.12 from syllabus
    float received_power_mw;
    if (PHASE) {
        received_power_mw = static_cast<float>(emission_factor.squared_norm() * (30 * P_TX * 0.13 * C * C) / (PI * R_A * FREQUENCY * FREQUENCY));
    }
    else {
        received_power_mw = static_cast<float>(emission_factor.get_real() * (30 * P_TX * 0.13 * C * C) / (PI * R_A * FREQUENCY * FREQUENCY));
    }
    float received_power_dBm = 10 * log10f(received_power_mw);
    if (received_power_dBm > -40.0f) {
        return 40e9;
    }
    else if (received_power_dBm < -90.0f) {
        return 0.0f;
    }
    float binary_rate_dB = static_cast<float>(76.9897 + ((received_power_dBm + 90) / 50 * (106.0206 - 76.9897)));
    return (pow(10.0f, binary_rate_dB / 10));
}

void RealAntenna::reset()
{
    for (Ray* r : rays) {
        delete r;
    }
    rays.clear();
    emission_factor = Complex(0.0f);
    min_attenuation = 0.0f;
    max_attenuation = 0.0f;
}

void VirtualAntenna::add_ray(Ray* r) {
    src->add_ray(r);
}

