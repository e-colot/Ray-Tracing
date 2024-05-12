#pragma once

#include "antenna.h"
#include <map>
#include <vector>
#include <mutex>

class Tile;

using doubleVect = std::vector<double>;

class Tile {
    // Attributes
private:
    const Vector pos; // Position of the tile
    doubleVect rates; // Rates associated with the tile
    RealAntenna* antenna; // Antenna associated with the tile
    std::mutex mtx; // to lock access while doing parallel operations

    // Constructors
public:
    Tile(); // Default constructor
    Tile(const Vector& center_of_tile); // Constructor with center position
    Tile(const Tile* origin); // Copy constructor from another Tile

    // Destructor
public:
    ~Tile(); // Destructor

    // Accessors
public:
    double get_rate(int position); // Returns the rate at a specific position within the tile (non const bcs of the mutex)
    RealAntenna* get_antenna() const; // Returns the antenna associated with the tile
    const Vector& get_pos() const; // Returns the position of the tile

    // Method
public:
    void add_rate(double rate); // Adds a rate to the tile
    void delete_rates(); // Clears the "rates" vector


};
