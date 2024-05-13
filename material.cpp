#include "material.h"
#include "math.h"
#include <iostream>

// Constructors

Material::Material() : eps(0.0f), col(color()), thickness(0.0f), propagation_cst(Complex()), impedance(Complex()) {}
Material::Material(float perm, float cond, const color& color, float thick) :
    eps(perm* EPS_0), col(color), thickness(thick),
    propagation_cst(Complex(PULS/C * sqrt(perm/2) * sqrt(sqrt(1 + std::pow(cond / (PULS * eps), 2.0)) - 1), PULS* sqrt((MU_0 * eps) / 2) * sqrt(sqrt(1 + std::pow(cond / (PULS * eps), 2.0)) + 1))),
    impedance(((static_cast<float>(MU_0)) / (Complex(eps, -cond / (PULS)))).cplx_sqrt()) {}

// Accessors

float Material::get_relative_permittivity() const {
    return eps / EPS_0;
}
Complex Material::get_impedance() const {
    return impedance;
}
Complex Material::get_propagation_cst() const {
    return propagation_cst;
}
color Material::get_color() const {
    return col;
}
float Material::get_thickness() const {
    return thickness;
}
