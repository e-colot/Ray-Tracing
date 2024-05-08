#pragma once

#include "wall.h"

using wallVect = std::vector<const Wall*>;

class Path {
    // Attributes
private:
    const Vector start; // Start point of the path
    const Vector end; // End point of the path

    // Constructors
public:
    Path(); // Default constructor
    Path(const Vector& start_point, const Vector& end_point); // Constructor with start and end points

    // Accessors
public:
    Vector get_start() const; // Returns the start point of the path
    Vector get_end() const; // Returns the end point of the path

    // Methods
public:
    void show() const; // Displays information about the path
    float intersect_wall(const Wall* wall_to_intersect) const; // Calculates intersection with a wall
    Complex calc_reflection(const Wall* wall_to_reflect_into, float intersection) const; // Calculates reflection off a wall
    double calc_attenuation(const wallVect& map) const; // Calculates attenuation along the path

private:
    Complex calc_transmission(const Wall* wall_to_transmit_through, float intersection, Complex* gamma_perp = nullptr, std::vector<float*> geometric_values = {}) const; // Calculates transmission through a wall
    Complex calc_gamma_perp(const Wall* w, float intersection, std::vector<float*> geometric_values) const; // returns in gamma_perp and geometric_values = { sin(\theta_i), sin(\theta_t), s }
};
