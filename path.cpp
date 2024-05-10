#include "path.h"
#include "math.h"
#include <iostream>

#define MIN(a, b) ((a < b) ? a : b)

// Constructors

Path::Path() : start(Vector()), end(Vector()) {}
Path::Path(const Vector& s, const Vector& e) : start(s), end(e) {}

// Accessors

Vector Path::get_start() const {
    return start;
}
Vector Path::get_end() const {
    return end;
}

// Methods

void Path::show() const {
    std::cout << "Path from (";
    start.show(false);
    std::cout << ") to (";
    end.show(false);
    std::cout << ")" << std::endl;
}
float Path::intersect_wall(const Wall* w) const {
    float res = -1.0f;
    if ((start - w->get_pos()).scalar_prod(w->get_normal()) * (end - w->get_pos()).scalar_prod(w->get_normal()) < 0.0f) {
        // if the wall is between the start and the end of the path
        float fraction = -((start - w->get_pos()).scalar_prod(w->get_normal())) / ((end - start).scalar_prod(w->get_normal()));
        // fraction of the wall at which it is crossed by the path
        float height = ((start - w->get_pos()).scalar_prod(w->get_dir())) + (fraction * ((end - start).scalar_prod(w->get_dir())));
        for (int i = 0; i < static_cast<int>(w->get_intervals().size()); i += 2) {
            if ((i + 1 != static_cast<int>(w->get_intervals().size())) && (height >= w->get_intervals()[i]) && (height <= w->get_intervals()[i + 1])) {
                // if the intersection is in the wall
                res = height;
                break;
            }
        }
    }
    else if (start == w->get_pos()) {
        // if the start of the wall is exactly at the start of the path
        /*
        ______________
            /|\
           / | \
          X  |  X
             |
        Where the "X" are the antennas and the walls are vertical and horizontal
        */
        res = 0.0f;
    }
    else if (start == (w->get_pos() + w->get_dir() * w->get_intervals().back())) {
        // if the end of the wall is exactly at the start of the path
        // (same situation)
        res = w->get_intervals().back();
    }
    return res;
}
Complex Path::calc_reflection(const Wall* w, float intersection) const {
    float sin_i = 0;
    float sin_t = 0;
    float s = 0;
    Complex gamma_perp = Complex();
    // 8.43 from syllabus
    // + 8.44 from syllabus
    Complex T_m = calc_transmission(w, intersection, &gamma_perp, { &sin_i, &sin_t, &s });
    Complex Gamma_m;
    if (T_m == Complex(0, 0)) {
        Gamma_m = gamma_perp;
        // looks logic but only God knows why when T_m = 0, Gamma_m = NaN + j*NaN        
    }
    else {
        Gamma_m = gamma_perp - T_m * (gamma_perp * cplx_exp(Complex(0.0f, -s + 2 * s * BETA_AIR * sin_i * sin_t) * w->get_material(intersection)->get_propagation_cst()));
    }
    if (gamma_perp.get_real() != gamma_perp.get_real()) {
        // happens if the ray is parallel to the wall -> no reflexion
        return Complex(0, 0);
    }
    return Gamma_m;
}
Complex Path::calc_transmission(const Wall* w, float intersection, Complex* gamma_p, std::vector<float*> geo) const {
    float sin_i = 0;
    float sin_t = 0;
    float s = 0;
    Complex gamma_perp = calc_gamma_perp(w, intersection, {&sin_i, &sin_t, &s});
    // 8.44 from syllabus
    if (gamma_p != nullptr) {
        *geo[0] = sin_i;
        *geo[1] = sin_t;
        *geo[2] = s;
        *gamma_p = gamma_perp;
    }
    if (gamma_perp.get_real() != gamma_perp.get_real()) {
        // happens if the ray is parallel to the wall -> full transmission
        return Complex(1, 0);
    }
    Complex T_m = (((1 - (gamma_perp * gamma_perp)) * cplx_exp(Complex(0.0f, - s) * w->get_material(intersection)->get_propagation_cst())) /
        (1 - gamma_perp * gamma_perp * cplx_exp(Complex(0.0f, - 2 * s) * w->get_material(intersection)->get_propagation_cst() +
            Complex(0.0f, BETA_AIR * 2 * s * sin_t * sin_i))));
    if (T_m.get_real() != T_m.get_real() || T_m.get_imag() != T_m.get_imag()) {
        // This condition is only true if T_m.get_real() == NaN
        // Happens if the propagation constant is too big (for metal)
        // Makes something that looks like 1/(e^(7000 - j7000)) = 1 / ((1-j) * 10^300)
        // So it is simplified to 0
        // Wich means that physically, no signal can pass through metal
        // Not a "clean" code but handling NaN might takes some time so this do the trick for the moment
        T_m = Complex(0, 0);
    }
    return T_m;
}
Complex Path::calc_gamma_perp(const Wall* w, float intersection, std::vector<float*> geo) const {
    float cos_i = MIN(abs((end - start).scalar_prod(w->get_normal())) / (end - start).get_norm(), 1.0f);
    float sin_i = sqrt(1 - cos_i * cos_i);
    float sin_t = sin_i * sqrt(1 / w->get_material(intersection)->get_relative_permittivity());
    float cos_t = sqrt(1 - sin_t * sin_t);
    // 8.39 from syllabus
    Complex gamma_perp = ((w->get_material(intersection)->get_impedance() * cos_i) - static_cast<float>(Z_0 * cos_t)) /
        ((w->get_material(intersection)->get_impedance() * cos_i) + static_cast<float>(Z_0 * cos_t));
    *geo[0] = sin_i;
    *geo[1] = sin_t;
    *geo[2] = w->get_material(intersection)->get_thickness() / cos_t;
    return gamma_perp;
}
double Path::calc_attenuation(const wallVect& walls) const {
    // determine the attenuation due to all the walls that are crossed
    Complex attenuation = Complex(1, 0);
    for (const Wall* w : walls) {
        float intersection = -1.0f;
        intersection = intersect_wall(w);
        if (intersection >= 0.0f) {
            // si on intersecte le mur i
            attenuation *= calc_transmission(w, intersection);
        }
    }
    return attenuation.squared_norm();
}
