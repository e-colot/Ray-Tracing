#include "antenna.h"
#include "math.h"
#include <iostream>

//                              <---------- ANTENNA ---------->

// Constructors

Antenna::Antenna() : pos(Vector()), src(nullptr), wall(nullptr) {}
Antenna::Antenna(const Vector& position, RealAntenna* source, const Wall* w) : pos(position), src(source), wall(w) {}

// Accessors

const Vector Antenna::get_pos() const {
    return pos;
}
RealAntenna* Antenna::get_src() const {
    return src;
}
const Wall* Antenna::get_wall() const {
    return wall;
}

// Method

void Antenna::create_ray(const Antenna* rx, const wallVect& walls) {
    if ((wall != rx->get_wall()) or ((wall == nullptr) && (rx->get_wall() == nullptr))) {
        // making sure it's not two virtual antennas mirrored on the same wall
        // the condition after the or is true if the two antennas are RealAntennas -> direct link
        Ray* new_ray_ptr = nullptr;
        Ray* new_ray = new Ray(this, rx, &new_ray_ptr, walls);
        if (new_ray_ptr != nullptr) {
            //  => if the ray can exist
            // <=> if the ray bounces on each wall
            add_ray(new_ray);
        }
        new_ray_ptr = nullptr;
    }
}



//                           <---------- REAL ANTENNA ---------->

// Constructors

RealAntenna::RealAntenna() : RealAntenna(Vector()) {}
RealAntenna::RealAntenna(const Vector& position) : Antenna(position, this, nullptr), emission_factor(0.0f) {}

// Destructor

RealAntenna::~RealAntenna() {
    for (int i = 0; i < static_cast<int>(virtual_network.size()); i++) {
        delete virtual_network[i];
    }
    for (int i = 0; i < static_cast<int>(rays.size()); i++) {
        delete rays[i];
    }
}

// Accessors

antennaVect RealAntenna::get_virtual_network() const {
    return virtual_network;
}
const rayVect RealAntenna::get_rays() const {
    return rays;
}

double RealAntenna::get_min_attenuation() const
{
    double res = 1e50;
    for (const Ray* r : rays) {
        if (r->get_attenuation() > 0.0 && r->get_attenuation() < res) {
            res = r->get_attenuation();
        }
    }
    return res;
}

double RealAntenna::get_max_attenuation() const
{
    double res = -1e50;
    for (const Ray* r : rays) {
        if (r->get_attenuation() > 0.0 && r->get_attenuation() > res) {
            res = r->get_attenuation();
        }
    }
    return res;
}

// Methods

void RealAntenna::virtualize(const Wall* w) {
    virtual_network.push_back(new VirtualAntenna(this, w));
}
void RealAntenna::add_ray(const Ray* r) {
    rays.push_back(r);
    emission_factor += r->get_attenuation();
}
double RealAntenna::get_binary_rate() const {
    double received_power_mw = get_power();
    double received_power_dBm = 10 * log10(received_power_mw);
    if (received_power_dBm > -40.0f) {
        return 40e9;
    }
    else if (received_power_dBm < -90.0f) {
        return 0.0f;
    }
    double binary_rate_dB = static_cast<double>(76.9897 + ((received_power_dBm + 90) / 50 * (106.0206 - 76.9897)));
    return (pow(10.0f, binary_rate_dB / 10));
}
double RealAntenna::get_power() const {
    // 4.5 from exercises syllabus
    // + 5.27 from syllabus
    // + 5.49 from syllabus with theta = pi/2
    // + 1.12 from syllabus
    double received_power_mw = static_cast<double>(emission_factor * (30 * P_TX * 0.13 * C * C) / (PI * R_A * FREQUENCY * FREQUENCY));
    // adding a max and a min to have a reasonable colormap
    if (received_power_mw >= 1e-2) {
        return 1e-2;
    }
    if (received_power_mw <= 1e-15) {
        return 1e-15;
    }
    return received_power_mw;
}
void RealAntenna::reset() {
    for (const Ray* r : rays) {
        delete r;
    }
    rays.clear();
    emission_factor = 0.0f;
}



//                          <---------- VIRTUAL ANTENNA ---------->

// Constructors

VirtualAntenna::VirtualAntenna() : Antenna() {}
VirtualAntenna::VirtualAntenna(RealAntenna* realAntenna, const Wall* reflectedWall) : Antenna(reflectedWall->mirror(realAntenna->get_pos()), realAntenna, reflectedWall) {}

// Method

void VirtualAntenna::add_ray(const Ray* r) {
    src->add_ray(r);
}

