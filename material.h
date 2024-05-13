#pragma once

#include "complex.h"

class Material {
    // Attributes
private:
    const float eps; // permittivity
    const color col; // Color of the material
    const float thickness; // Thickness of the material
    const Complex propagation_cst; // Propagation constant (gamma_m in syllabus)
    const Complex impedance; // Impedance (Z_m in syllabus)

    // Constructors
public:
    Material(); // Default constructor
    Material(float relative_permittivity, float conductivity, const color& color, float thickness); // Constructor with parameters

    // Accessors
public:
    float get_relative_permittivity() const; // Returns the relative permittivity
    Complex get_propagation_cst() const; // Returns the propagation constant
    Complex get_impedance() const; // Returns the impedance
    color get_color() const; // Returns the color of the material
    float get_thickness() const; // Returns the thickness of the material
};
