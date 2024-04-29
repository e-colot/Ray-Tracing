#include "material.h"
#include "math.h"
#include <iostream>

// ---------- CONSTRUCTORS ----------

Material::Material() : eps(0.0f), sigma(0.0f), col(color()) {}
Material::Material(float perm, float conv, float thick) : eps(perm* EPS_0), sigma(conv), col(color()), thickness(thick) {
    float re_prop_cst = PULS * sqrt((MU_0 * eps) / 2) * sqrt(sqrt(1 + (sigma * sigma / (PULS * PULS * eps * eps))) - 1);
    float im_prop_cst = PULS * sqrt((MU_0 * eps) / 2) * sqrt(sqrt(1 + (sigma * sigma / (PULS * PULS * eps * eps))) + 1);
    propagation_cst = Complex(re_prop_cst, im_prop_cst);
    impedance = ((static_cast<float>(MU_0)) / (Complex(eps, -sigma / (PULS)))).cplx_sqrt();
}
Material::Material(float perm, float conv, color color, float thick) : eps(perm* EPS_0), sigma(conv), col(color), thickness(thick) {
    float re_prop_cst = PULS * sqrt((MU_0 * eps) / 2) * sqrt(sqrt(1 + (sigma * sigma / (PULS * PULS * eps * eps))) - 1);
    float im_prop_cst = PULS * sqrt((MU_0 * eps) / 2) * sqrt(sqrt(1 + (sigma * sigma / (PULS * PULS * eps * eps))) + 1);
    propagation_cst = Complex(re_prop_cst, im_prop_cst);
    impedance = ((static_cast<float>(MU_0)) / (Complex(eps, -sigma / (PULS)))).cplx_sqrt();
}

// ---------- ACCESSORS ----------

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

float Material::get_thickness() const
{
    return thickness;
}
