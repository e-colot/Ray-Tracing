#pragma once

#include "graphics.h"
#include "tile.h"
#include <vector>

class Map;

using cornerVect = std::vector<const corner*>;
using floatMatrix = std::vector<std::vector<float>>;

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
    void show_data_rate(const Vector& antenna_pos, bool dBm = false); // Shows data rate at a specific router position
    void show_data_rate(const Vector& antenna1_pos, const Vector& antenna2_pos, bool dBm = false); // Shows data rate with two routers
    void optimize_placement(int number_of_antenna); // Optimizes antenna placement

private:
    void show_map() const; // Shows the map in the window
    void add_wall(const Wall* wall_to_add); // Adds a wall to the map
    void add_corner(const corner* corner_to_add); // Adds a corner to the map
    void setup_materials(); // Sets up materials for the map
    void setup_walls(bool lift); // Sets up walls on the map
    void virtualize_antenna(RealAntenna* antenna) const; // Virtualizes an antenna
    void create_rays() const; // Creates rays on the map
    void calculate_data_rate(); // Calculates data rate on the map
    void calculate_data_rate(Tile* tx_tile); // Calculates data rate for a specific tile as emmiter
    void calculate_data_rate(tileVect* tx_tiles); // Calculates data rate for multiple tiles as emitters
    void setup_tiles(float tile_size = TILE_SIZE); // Sets up tiles on the map
    Tile* find_closest_tile(const Vector& position) const; // Finds the closest tile to a given position
    void setup_accessible_tiles(); // Sets up accessible tiles
    double calc_rate() const; // Calculates the data rate between rx and tx
};
