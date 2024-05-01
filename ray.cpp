#include "ray.h"
#include "antenna.h"
#include <iostream>

// Constructors

Ray::Ray() : reflects({ Vector() }), tx(nullptr), rx(nullptr), start(Vector()), translation(Vector()), path({ nullptr }), attenuation(1.0f) {}
Ray::Ray(const Antenna* t, const Antenna* r, Ray** ptr, const wallVect& walls) : tx(t), rx(r), start(t->get_pos()), translation(r->get_pos() - t->get_pos()), attenuation(1.0f) {
    Path* virtual_path = new Path(tx->get_pos(), rx->get_pos());
    if (exist(virtual_path)) {
        *ptr = this;
        // the following line adds the exponential and the decrease with distance terms of 4.5 from exercises syllabus
        // attenuation = cplx_exp(Complex(0.0f, -(BETA_AIR)*translation.get_norm())) / translation.get_norm();
        // it is however simplified because we don't take the phase into account 
        attenuation = 1 / (translation.squared_norm());
        create_path(walls);
    }
    delete virtual_path;
    virtual_path = nullptr;
}

// Destructor

Ray::~Ray() {
    for (int i = 0; i < static_cast<int>(path.size()); i++) {
        delete path[i];
        path[i] = nullptr;
    }
    tx = nullptr;
    rx = nullptr;
}

// Accessors

const Vector Ray::get_start() const {
    return start;
}
const Vector Ray::get_translation() const {
    return translation;
}
const Antenna* Ray::get_rx() const {
    return rx;
}
const Antenna* Ray::get_tx() const {
    return tx;
}
const vectorVect Ray::get_reflects() const {
    return reflects;
}
const pathVect Ray::get_path() const {
    return path;
}
double Ray::get_attenuation() const {
    return attenuation;
}

// Methods

void Ray::show() const {
    for (const Path* p : path) {
        p->show();
    }
}
void Ray::add_reflect(const Vector& r) {
    reflects.push_back(r);
}
bool Ray::exist(Path* virtual_path) {
    bool first_bounce = false;
    add_reflect(tx->get_src()->get_pos());
    if (tx->get_wall() != nullptr) {
        float intersection = virtual_path->intersect_wall(tx->get_wall());
        if (intersection != NULL) {
            first_bounce = true;
            attenuation *= virtual_path->calc_reflection(tx->get_wall(), intersection).squared_norm();
            add_reflect(tx->get_wall()->get_pos() + tx->get_wall()->get_dir() * intersection);
        }
        else {
            return false;
        }
    }
    if (rx->get_wall() != nullptr) {
        float intersection = virtual_path->intersect_wall(rx->get_wall());
        if (intersection != NULL) {
            add_reflect(rx->get_wall()->get_pos() + rx->get_wall()->get_dir() * intersection);
            if (first_bounce) {
                if ((tx->get_pos() - reflects[2]).squared_norm() < (tx->get_pos() - reflects[1]).squared_norm()) {
                    // if we are in a specific case :
                    /*
                           X
                          /
                         /
                        /     X
                       /     / 
                    ------  /
                       \ | /
                        \|/
                         |    
                    Where the "X" are the antennas and the walls are vertical and horizontal
                    */
                    return false;
                }
            }
            attenuation *= virtual_path->calc_reflection(rx->get_wall(), intersection).squared_norm();
        }
        else {
            return false;
        }
    }
    add_reflect(rx->get_src()->get_pos());
    return true;
}
void Ray::create_path(const wallVect& walls) {
    for (int i = 0; i < static_cast<int>((reflects.size()) - 1); i++) {
        Path* new_path = new Path(reflects[i], reflects[i + 1]);
        attenuation *= new_path->calc_attenuation(walls);
        add_path(new_path);
    }
}
void Ray::add_path(Path* p) {
    path.push_back(p);
}
