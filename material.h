#pragma once

#include "complex.h"

class Material;

class Material {

    // ---------- CONSTRUCTORS ----------

public:
    Material();
    Material(float relative_permittivity, float conductivity, const color& color, float thickness);

    // ---------- ACCESSORS ----------

public:
    float get_relative_permittivity() const;
    Complex get_propagation_cst() const;
    Complex get_impedance() const;
    color get_color() const;
    float get_thickness() const;

    // ---------- ATTRIBUTES ----------

private:
    const float eps;
    const float sigma;
    const color col;
    const float thickness;
    const Complex propagation_cst;        // gamma_m in syllabus
    const Complex impedance;              // Z_m in syllabus
};
