#pragma once

#include "graphics.h"
#include "tile.h"
#include <vector>

class Map;

using cornerVect = std::vector<const corner*>;
using floatMatrix = std::vector<std::vector<float>>;
using realantennaVect = std::vector<RealAntenna*>;
using intVect = std::vector<int>;

class Map {
    friend Map; // allows the main Map to call private function (brut force and gradient descent) of other maps
    // Attributes
private:
    const Material* exo_4_1; // Material for exercise 4.1
    const Material* concrete; // Material for concrete
    const Material* gyproc; // Material for gyproc
    const Material* glass; // Material for glass
    const Material* metal; // Material for metal
    Graphics* display; // Display graphics
    wallVect walls; // Walls on the map
    cornerVect corners; // Corners on the map
    tileVect tiles; // Tiles on the map
    RealAntenna* rx; // Receiving antenna
    RealAntenna* tx; // Transmitting antenna

    // Constructors
public:
    Map(); // Default constructor
    Map(Graphics* window); // Constructor with Graphics window

    // Destructor
public:
    ~Map(); // Destructor

    // Mutator
public:
    void add_window(Graphics* window); // Adds a window

    // Methods
public:
    void show_rays(Vector tx_pos, Vector rx_pos, bool logarithmic = false); // Shows rays on the map
    void show_data_rate(const vectorVect& antenna_pos, bool dBm = false, float tilesize = TILE_SIZE); // Shows data rate with multiple routers
    void optimize_placement(int number_of_antenna); // Optimizes antenna placement

private:
    void show_map() const; // Shows the walls and the corners in the window
    void add_wall(const Wall* wall_to_add); // Adds a wall to the list of walls
    void add_corner(const corner* corner_to_add); // Adds a corner to the list of corners
    void setup_materials(); // Sets up materials for the map
    void setup_walls(bool lift); // Sets up walls on the map
    void virtualize_antenna(RealAntenna* antenna) const; // Virtualizes an antenna
    void create_rays() const; // Creates rays on the map
    void calculate_data_rate(); // Calculates data rate with an router on each accessible_tile
    void calculate_data_rate(const realantennaVect& tx_antenna); // Calculates data rate for the given routers
    Tile* find_closest_tile(const Vector& position) const; // Finds the closest tile to a given position
    void setup_tiles(float tile_size, bool restrained); // Sets up tiles on the map, restrained don't add tiles in the walls
    double calc_rate() const; // Calculates the data rate between rx and tx

    vectorVect best_position(int nbr_antennas) const;
    vectorVect brut_force(int number_of_antenna, float tile_size); // Search best tile to place the router(s) by trying every accessible tile
    void gradient_descent(vectorVect* pos, float tile_size, float precision);
    int best_of_3() const;
};
