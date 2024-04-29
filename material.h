#pragma once

#include "complex.h"

class Material;

class Material {

    // ---------- CONSTRUCTORS ----------

public:
    Material();
    Material(float relative_permittivity, float conductivity, float thickness);
    Material(float relative_permittivity, float conductivity, color color, float thickness);

    // ---------- ACCESSORS ----------

public:
    float get_relative_permittivity() const;
    Complex get_propagation_cst() const;
    Complex get_impedance() const;
    color get_color() const;
    float get_thickness() const;

    // ---------- ATTRIBUTES ----------

private:
    float eps;
    float sigma;
    Complex propagation_cst;        // gamma_m in syllabus
    Complex impedance;              // Z_m in syllabus
    color col;
    float thickness;
};
