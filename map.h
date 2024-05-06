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
    tileVect accessible_tiles; // Accessible tiles on the map
    RealAntenna* rx; // Receiving antenna
    RealAntenna* tx; // Transmitting antenna

    // Constructors
public:
    Map(); // Default constructor
    Map(Graphics* window); // Constructor with Graphics window
    Map(const Vector& tx_pos, const Vector& rx_pos); // Constructor with antenna positions
    Map(const Vector& tx_pos, const Vector& rx_pos, Graphics* window); // Constructor with antenna positions and Graphics window

    // Destructor
public:
    ~Map(); // Destructor

    // Mutator
public:
    void add_window(Graphics* window); // Adds a window

    // Methods
public:
    void show_rays(bool logarithmic = false) const; // Shows rays on the map
    void show_data_rate(const vectorVect& antenna_pos, bool dBm = false, float tilesize = TILE_SIZE); // Shows data rate with multiple routers
    void optimize_placement(int number_of_antenna); // Optimizes antenna placement

    vectorVect brut_force(int number_of_antenna, float tile_size); // Search best tile to place the router(s) by trying every accessible tile
    void gradient_descent(vectorVect* pos, float tile_size, float precision);

private:
    int best_direction(const floatMatrix& direction_values); // determines the best direction for the gradient descent
    void show_map() const; // Shows the map in the window
    void add_wall(const Wall* wall_to_add); // Adds a wall to the map
    void add_corner(const corner* corner_to_add); // Adds a corner to the map
    void setup_materials(); // Sets up materials for the map
    void setup_walls(bool lift); // Sets up walls on the map
    void virtualize_antenna(RealAntenna* antenna) const; // Virtualizes an antenna
    void create_rays() const; // Creates rays on the map
    void calculate_data_rate(); // Calculates data rate on the map
    void calculate_data_rate(Tile* tx_tile); // Calculates data rate for a specific tile as emmiter
    void calculate_data_rate(const realantennaVect& tx_antenna); // Calculates data rate for multiple antennas as emitters
    void calculate_data_rate(const tileVect& tx_tiles); // Calculates data rate for multiple tiles as emitters
    floatVect calculate_coverage_and_rate(const realantennaVect& tx_antenna);
    void setup_tiles(float tile_size = TILE_SIZE); // Sets up tiles on the map
    Tile* find_closest_tile(const Vector& position) const; // Finds the closest tile to a given position
    void setup_accessible_tiles(); // Sets up accessible tiles
    double calc_rate() const; // Calculates the data rate between rx and tx
    void clean_accessible_tiles_data() const; // Deletes the rates calculated for the accessible tiles
};
