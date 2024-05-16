#pragma once

#include "graphics.h"
#include "tile.h"
#include <vector>

class Map;

using cornerVect = std::vector<const corner*>;
using floatMatrix = std::vector<std::vector<float>>;
using realantennaVect = std::vector<RealAntenna*>;

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
    bool lift; // Is the lift there

    // Constructors
public:
    Map(bool lift = false); // Default constructor
    Map(Graphics* window, bool lift = false); // Constructor with Graphics window

    // Destructor
public:
    ~Map(); // Destructor

    // Methods
public:
    void show_rays(Vector tx_pos, Vector rx_pos, float tile_size, bool logarithmic); // Shows rays on the map
    void show_data_rate(const vectorVect& antenna_pos, bool dBm, float tilesize); // Shows data rate with multiple routers
    void show_power(const vectorVect& antenna_pos, bool dBm, float tilesize); // Shows power with multiple routers
    void optimize_placement(int number_of_antenna, float precision, char precision_level, bool dBm); // Optimizes antenna placement

private:
    // utility
    void show_map() const; // Shows the walls and the corners in the window
    void add_wall(const Wall* wall_to_add); // Adds a wall to the list of walls
    void add_corner(const corner* corner_to_add); // Adds a corner to the list of corners
    void setup_materials(); // Sets up materials for the map
    void setup_walls(); // Sets up walls on the map
    void setup_tiles(float tile_size, bool restrained); // Sets up tiles on the map. If restrained, don't add tiles in the walls and in the doors
    // components creation
    void virtualize_antenna(RealAntenna* antenna) const; // Virtualizes an antenna
    void create_rays(RealAntenna* tx, const RealAntenna* rx) const; // Creates rays on the map
    // calculations
    void calculate_data_rate(); // Calculates data rate with an router on each accessible_tile
    void calculate_data_rate(const realantennaVect& tx_antenna); // Calculates data rate for the given routers
    void calculate_power(const realantennaVect& tx_antenna); // Calculates power for the given routers
    double calc_rate(RealAntenna* tx, const RealAntenna* rx) const; // Calculates the data rate between tx and rx
    double calc_power(RealAntenna* tx, const RealAntenna* rx) const; // Calculates the power received by rx emitted by tx
    // optimization
    vectorVect brut_force(int number_of_antenna, float tile_size); // Search best tile to place the router(s) by trying every accessible tile
    void gradient_descent(vectorVect* pos, float tile_size, float precision);
    vectorVect best_position(int nbr_antennas) const; // returns the best positions for the given number of routers
    int best_of_3(floatVect* best) const; // returns the best direction (by index) and stores the parametres in best
    bool best_of_2(floatVect* best) const; // returns (new pos >= last_pos)
};
