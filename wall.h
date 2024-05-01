#pragma once

#include "material.h"
#include "vector.h"
#include <vector>

class Wall;

using floatVect = std::vector<float>;
using matVect = std::vector<const Material*>;

class Wall {
    // Attributes
private:
    const Vector pos; // Position of the wall
    Vector dir; // Direction of the wall
    Vector normal; // Normal vector of the wall
    const floatVect intervals; // Interval values along the wall
    const matVect materials; // Materials along the wall

    // Constructors
public:
    Wall(); // Default constructor
    Wall(const Vector& start_position, const Vector& direction, const floatVect& interval_values, const Material* material); // Constructor with single material
    Wall(const Vector& start_position, const Vector& direction, const floatVect& interval_values, const matVect materials); // Constructor with multiple materials

    // Accessors
public:
    Vector get_pos() const; // Returns the position of the wall
    Vector get_normal() const; // Returns the normal vector of the wall
    Vector get_dir() const; // Returns the direction vector of the wall
    floatVect get_intervals() const; // Returns the interval values along the wall
    const Material* get_material(float length) const; // Returns the material at a specific length along the wall
    const Material* get_material(int section) const; // Returns the material at a specific section of the wall

    // Methods
public:
    Vector mirror(const Vector& position_to_reflect) const; // Reflects a vector off the wall
    bool inside(const Vector& pos) const; // Checks if a point is inside the wall
};
