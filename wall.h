#pragma once

#include "material.h"
#include "vector.h"
#include <vector>

class Wall;

using floatVect = std::vector <float>;
using matVect = std::vector<Material*>;

class Wall {

    // ---------- CONSTRUCTORS ----------

public:
    Wall();
    Wall(const Vector& start_position, const Vector& direction, const floatVect& interval_values,
        Material* material);
    Wall(const Vector& start_position, const Vector& direction, const floatVect& interval_values,
        matVect materials);

    // ---------- ACCESSORS ----------

public:
    Vector get_pos() const;
    Vector get_normal() const;
    Vector get_dir() const;
    floatVect get_intervals() const;
    Material* get_material(float length) const;
    Material* get_material(int section) const;

    // ---------- METHODS ----------

public:
    Vector mirror(const Vector& position_to_reflect) const;

    // ---------- ATTRIBUTES ----------        

private:
    Vector pos;
    Vector dir;
    Vector normal;
    floatVect intervals;
    matVect material;

};
