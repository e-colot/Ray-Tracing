#pragma once

#include <vector>
#include "wall.h"
#include "path.h"

class Ray;
class Antenna; // here to allow a ray to know that a class Antenna exists to have an instance of it as its attributes

using vectorVect = std::vector<Vector>;
using pathVect = std::vector<const Path*>;

class Ray {

    // ---------- CONSTRUCTORS ----------

public:
    Ray();
    Ray(const Antenna* emitter, const Antenna* receiver, Ray** dbl_ptr_to_this, const wallVect& all_walls);

    // ---------- DESTRUCTORS ----------

public:
    ~Ray();

    // ---------- ACCESSORS ----------

public:
    const Vector get_start() const;
    const Vector get_translation() const;
    const Antenna* get_rx() const;
    const Antenna* get_tx() const;
    const vectorVect get_reflects() const;
    const pathVect get_path() const;
    float get_attenuation() const;

    // ---------- METHODS ----------

public:
    void show() const;
private:
    void add_reflect(const Vector& reflect);
    bool exist(Path* virtual_path);
    void create_path(const wallVect& all_walls);
    void add_path(Path* path);

    // ---------- ATTRIBUTES ----------

private:
    vectorVect reflects;
    const Antenna* tx;
    const Antenna* rx;
    const Vector start;
    const Vector translation;
    pathVect path;
    float attenuation;

};
