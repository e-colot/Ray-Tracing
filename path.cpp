#include "path.h"
#include "math.h"
#include <iostream>

// ---------- CONSTRUCTORS ----------

Path::Path() : start(Vector()), end(Vector()) {}
Path::Path(const Vector& s, const Vector& e) : start(s), end(e) {}

// ---------- ACCESSORS ----------

Vector Path::get_start() const {
    return start;
}
Vector Path::get_end() const {
    return end;
}

// ---------- METHODS ----------

void Path::show() const {
    std::cout << "Path from (";
    start.show(false);
    std::cout << ") to (";
    end.show(false);
    std::cout << ")" << std::endl;
}
float Path::intersect_wall(const Wall* w) const {
    float res = NULL;
    if ((start - w->get_pos()).scalar_prod(w->get_normal()) * (end - w->get_pos()).scalar_prod(w->get_normal()) < 0.0f) {
        // if the wall is between the start and the end of the path
        float fraction = -((start - w->get_pos()).scalar_prod(w->get_normal())) / ((end - start).scalar_prod(w->get_normal()));
        float height = ((start - w->get_pos()).scalar_prod(w->get_dir())) + (fraction * ((end - start).scalar_prod(w->get_dir())));
        for (int i = 0; i < static_cast<int>(w->get_intervals().size()); i += 2) {
            if ((i + 1 != static_cast<int>(w->get_intervals().size())) && (height >= w->get_intervals()[i]) && (height <= w->get_intervals()[i + 1])) {
                // if the intersection is in the wall
                res = height;
                break;
            }
        }
    }
    return res;
}
Complex Path::calc_reflection(const Wall* w, float intersection) const {
    float cos_i = abs((end - start).scalar_prod(w->get_normal())) / (end - start).get_norm();
    float sin_i = sqrt(1 - cos_i * cos_i);
    float sin_t = sin_i * sqrt(1 / w->get_material(intersection)->get_relative_permittivity());
    float cos_t = sqrt(1 - sin_t * sin_t);
    // 8.39 from syllabus
    return (((w->get_material(intersection)->get_impedance() * cos_i) - static_cast<float>(Z_0 * cos_t)) /
        ((w->get_material(intersection)->get_impedance() * cos_i) + static_cast<float>(Z_0 * cos_t)));
}
Complex Path::calc_transmission(const Wall* w, float intersection) const {
    float cos_i = abs((end - start).scalar_prod(w->get_normal())) / (end - start).get_norm();
    float sin_i = sqrt(1 - cos_i * cos_i);
    float sin_t = sin_i * sqrt(1 / w->get_material(intersection)->get_relative_permittivity());
    float cos_t = sqrt(1 - sin_t * sin_t);
    Complex gamma_perp = ((w->get_material(intersection)->get_impedance() * cos_i) - static_cast<float>(Z_0 * cos_t)) /
        ((w->get_material(intersection)->get_impedance() * cos_i) + static_cast<float>(Z_0 * cos_t));
    float s = w->get_material(intersection)->get_thickness() / cos_t;
    // 8.44 from syllabus
    Complex T_perp = (((1 - (gamma_perp * gamma_perp)) * cplx_exp(-s * w->get_material(intersection)->get_propagation_cst())) /
        (1 - gamma_perp * gamma_perp * cplx_exp(-2 * w->get_material(intersection)->get_propagation_cst() * s +
            Complex(0.0f, BETA_AIR * 2 * s * sin_t * sin_i))));
    if (T_perp.get_real() != T_perp.get_real()) {
        // This condition is only true if T_perp.get_real() == NaN
        // Happens if the propagation constant is too big (for metal)
        // Makes something that looks like 1/(e^(7000 - j7000)) = 1 / ((1-j) * 10^300)
        // So I simplify it to 0
        // Wich means that physically, no signal can pass through metal
        // Not a "clean" code but I don't want to handle Nan in Complex
        T_perp = Complex(0, 0);
    }
    return T_perp;
}
Complex Path::calc_attenuation(const wallVect& walls) const {
    Complex attenuation = Complex(1, 0);
    for (Wall* w : walls) {
        float intersection = intersect_wall(w);
        if (intersection != NULL) {
            // si on intersecte le mur i
            attenuation *= calc_transmission(w, intersection);
        }
    }
    return attenuation;
}
