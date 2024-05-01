#include "wall.h"
#include <vector>
#include <iostream>

#define ABS(a) ((a>=0)? a : -a)

// ---------- CONSTRUCTORS ----------

Wall::Wall() :
    pos(Vector()), dir(Vector()), normal(Vector()), intervals(floatVect{ 0.0f }), materials({ nullptr }) {}
Wall::Wall(const Vector& start_pos, const Vector& direction, const floatVect& int_val, const Material* mat) : 
    pos(start_pos), dir(direction), intervals(int_val), materials({ mat }) {
    dir.normalize();
    normal = dir.perpendicular();
    normal.normalize();
}
Wall::Wall(const Vector& start_pos, const Vector& direction, const floatVect& int_val, const matVect mat) 
    : pos(start_pos), dir(direction), intervals(int_val), materials(mat) {
    dir.normalize();
    normal = dir.perpendicular();
    normal.normalize();
}

// ---------- ACCESSORS ----------

Vector Wall::get_pos() const {
    return pos;
}
Vector Wall::get_normal() const {
    return normal;
}
Vector Wall::get_dir() const {
    return dir;
}
floatVect Wall::get_intervals() const {
    return intervals;
}
const Material* Wall::get_material(float length) const {
    int section = 0;
    for (int i = 0; i < static_cast<int>(intervals.size()); i += 2) {
        if ((i + 1 != static_cast<int>(intervals.size())) && (length > intervals[i]) && (length < intervals[i + 1])) {
            section = i/2;
            break;
        }
    }
    return materials[section];
}
const Material* Wall::get_material(int section) const
{
    return materials[static_cast<int>(section/2)];
}

// ---------- METHODS ----------

Vector Wall::mirror(const Vector& initial) const {
    return initial - normal * 2 * ((initial - pos).scalar_prod(normal));
}

bool Wall::inside(const Vector& position) const
{
    float projection = (position - pos).scalar_prod(dir);
    if (projection < 0 || projection > intervals.back()) {
        return false;
    }
    Vector projected_point = pos + dir * projection;
    return (ABS((position - projected_point).scalar_prod(normal)) <= 1e-3);
}
