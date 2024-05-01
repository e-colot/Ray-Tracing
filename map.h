#pragma once

#include "graphics.h"
#include "tile.h"
#include <vector>

class Map;

using cornerVect = std::vector<corner*>;
using floatMatrix = std::vector<std::vector<float>>;

class Map {

    // ---------- CONSTRUCTORS ----------

public:
    Map();
    Map(Graphics* window);
    Map(const Vector& tx_pos, const Vector& rx_pos);
    Map(const Vector& tx_pos, const Vector& rx_pos, Graphics* window);

    // ---------- DESTRUCTORS ----------

public:
    ~Map();

    // ---------- MUTATORS ----------

public:
    void add_window(Graphics* window);

    // ---------- METHODS ----------

public:
    void show_rays();
    void show_data_rate(const Vector& antenna_pos, bool dBm = false);
    void show_data_rate(const Vector& antenna1_pos, const Vector& antenna2_pos, bool dBm = false);
    void get_binary_rate() const;
    void optimize_placement(int number_of_antenna);
private:
    void show_map() const;
    void add_wall(Wall* wall_to_add);
    void add_corner(corner* corner_to_add);
    void setup_materials();
    void setup_walls(bool lift);
    void virtualize_antenna(RealAntenna* antenna);
    void create_rays();
    void calculate_data_rate();
    void calculate_data_rate(Tile* tx_tile);
    void calculate_data_rate(tileVect* tx_tiles);
    void setup_tiles(float tile_size = TILE_SIZE);
    Tile* find_closest_tile(const Vector& position) const;
    void setup_accessible_tiles();
    float calc_rate();

    // ---------- ATTRIBUTES ----------

private:
    const Material* exo_4_1;
    const Material* concrete;
    const Material* gyproc;
    const Material* glass;
    const Material* metal;
    Graphics* display;
    wallVect walls;
    cornerVect corners;
    tileVect tiles;
    tileVect accessible_tiles;
    RealAntenna* rx;
    RealAntenna* tx;
};
