#pragma once

#include <vector>
#include "wall.h"
#include "path.h"

class Ray;
class Antenna; // Forward declaration to allow a ray to know that a class Antenna exists

using vectorVect = std::vector<Vector>;
using pathVect = std::vector<const Path*>;

class Ray {
    // Attributes
private:
    vectorVect reflects; // Stores intermediate positions
    const Antenna* tx; // Transmitting antenna
    const Antenna* rx; // Receiving antenna
    const Vector start; // Start point of the ray (pos of the emitter, which can be a virtual antenna)
    const Vector translation; // From tx to rx
    pathVect path; // Paths of the ray
    double attenuation; // Attenuation of the ray

    // Constructors
public:
    Ray(); // Default constructor
    Ray(const Antenna* emitter, const Antenna* receiver, Ray** dbl_ptr_to_this, const wallVect& all_walls); // Constructor with parameters (always used)

    // Destructor
public:
    ~Ray(); // Destructor

    // Accessors
public:
    const Vector get_start() const; // Returns the start point of the ray
    const Vector get_translation() const; // Returns the translation vector of the ray
    const Antenna* get_rx() const; // Returns the receiving antenna
    const Antenna* get_tx() const; // Returns the transmitting antenna
    const pathVect get_path() const; // Returns the paths of the ray
    double get_attenuation() const; // Returns the attenuation of the ray

    // Methods
public:
    void show() const; // Displays information about the ray

private:
    void add_reflect(const Vector& reflect); // Adds a reflection position to the ray
    bool exist(Path* virtual_path); // Checks if a virtual path could exist
    void create_path(const wallVect& all_walls); // Creates a path for the ray
    void add_path(Path* path); // Adds a path to the ray's path list
};
