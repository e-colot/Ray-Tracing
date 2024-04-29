#pragma once

#include "wall.h"

class Path;
using wallVect = std::vector<Wall*>;

class Path {

    // ---------- CONSTRUCTORS ----------

public:
    Path();
    Path(const Vector& start_point, const Vector& end_point);

    // ---------- ACCESSORS ----------

public:
    Vector get_start() const;
    Vector get_end() const;

    // ---------- METHODS ----------

public:
    void show() const;
    float intersect_wall(Wall* wall_to_intersect) const;
    Complex calc_reflection(Wall* wall_to_reflect_into, float intersection) const;
    Complex calc_attenuation(const wallVect& map) const;
private:
    Complex calc_transmission(Wall* wall_to_transmit_through, float intersection) const;

    // ---------- ATTRIBUTES ----------

private:
    Vector start;
    Vector end;
};
