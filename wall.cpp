#include "wall.h"
#include <vector>
#include <iostream>

// ---------- CONSTRUCTORS ----------

Wall::Wall() :
    pos(Vector()), dir(Vector()), intervals(floatVect{ 0.0f }) {
    material.push_back(new Material());
    normal = dir.perpendicular();
}
Wall::Wall(const Vector& start_pos, const Vector& direction, const floatVect& int_val, Material* mat)
    : pos(start_pos), dir(direction), intervals(int_val) {
    material.push_back(mat);
    dir.normalize();
    normal = dir.perpendicular();
    normal.normalize();
}
Wall::Wall(const Vector& start_pos, const Vector& direction, const floatVect& int_val, matVect mat) 
    : pos(start_pos), dir(direction), intervals(int_val), material(mat) {
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
Material* Wall::get_material(float length) const {
    int section = 0;
    for (int i = 0; i < static_cast<int>(intervals.size()); i += 2) {
        if ((i + 1 != static_cast<int>(intervals.size())) && (length > intervals[i]) && (length < intervals[i + 1])) {
            section = i/2;
            break;
        }
    }
    return material[section];
}
Material* Wall::get_material(int section) const
{
    return material[static_cast<int>(section/2)];
}

// ---------- METHODS ----------

Vector Wall::mirror(const Vector& initial) const {
    return initial - normal * 2 * ((initial - pos).scalar_prod(normal));
}
