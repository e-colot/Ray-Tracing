#pragma once

#include <vector>
#include "wall.h"
#include "path.h"

class Ray;
class Antenna;

using vectorVect = std::vector<Vector>;
using pathVect = std::vector<Path*>;

class Ray {

    // ---------- CONSTRUCTORS ----------

public:
    Ray();
    Ray(Antenna* emitter, Antenna* receiver, Ray** dbl_ptr_to_this, const wallVect& all_walls);

    // ---------- DESTRUCTORS ----------

public:
    ~Ray();

    // ---------- ACCESSORS ----------

public:
    Vector get_start() const;
    Vector get_translation() const;
    Antenna* get_rx() const;
    Antenna* get_tx() const;
    vectorVect get_reflects() const;
    pathVect get_path() const;
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
    Antenna* tx;
    Antenna* rx;
    Vector start;
    Vector translation;
    pathVect path;
    float attenuation;

};
