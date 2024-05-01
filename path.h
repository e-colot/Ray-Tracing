#pragma once

#include "wall.h"

class Path;
using wallVect = std::vector<const Wall*>;

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
    float intersect_wall(const Wall* wall_to_intersect) const;
    Complex calc_reflection(const Wall* wall_to_reflect_into, float intersection) const;
    float calc_attenuation(const wallVect& map) const;
private:
    Complex calc_transmission(const Wall* wall_to_transmit_through, float intersection) const;

    // ---------- ATTRIBUTES ----------

private:
    const Vector start;
    const Vector end;
};
