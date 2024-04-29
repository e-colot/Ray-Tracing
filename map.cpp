#include "map.h"
#include <iostream>
#include <chrono>

// ---------- CONSTRUCTORS ----------

Map::Map() {}
Map::Map(Graphics* g) {
    setup_materials();
    setup_walls(false);
    setup_tiles();
    add_window(g);
}
Map::Map(const Vector& tx_pos, const Vector& rx_pos)  {
    setup_materials();
    setup_walls(false);
    tx = new RealAntenna(tx_pos);
    rx = new RealAntenna(rx_pos);
}
Map::Map(const Vector& tx_pos, const Vector& rx_pos, Graphics* g) : Map(tx_pos, rx_pos) {
    add_window(g);
}

// ---------- DESTRUCTORS ----------

Map::~Map() {
    display = nullptr;
    rx = nullptr;
    tx = nullptr;
    delete exo_4_1;
    delete air;
    delete concrete;
    delete gyproc;
    delete glass;
    delete metal;
    for (Wall* i : walls) {
        delete i;
    }
    for (corner* i : corners) {
        delete i;
    }
    for (Tile* t : tiles) {
        delete t;
    }
    for (Tile* t : accessible_tiles) {
        delete t;
    }
}

// ---------- MUTATORS ----------

void Map::add_window(Graphics* g) {
    display = g;
}

// ---------- METHODS ----------

void Map::show_rays() {
    if (display == nullptr) {
        std::cout << "No window given to show the rays" << std::endl;
        return;
    }
    virtualize_antenna(rx);
    virtualize_antenna(tx);
    create_rays();
    show_map(display);
    display->add_rays(tx);
    for (Ray* r : tx->get_rays()) {
        r->show();
        std::cout << std::endl;
    }
}
void Map::show_data_rate(Vector pos) {
    if (display == nullptr) {
        std::cout << "No window given to show the data rate" << std::endl;
        return;
    }
    Tile* this_tile = find_closest_tile(pos);
    calculate_data_rate(this_tile);
    show_map(display);
    display->add_tiles(0, tiles);
}
void Map::get_binary_rate() const {
    std::cout << "Binary rate : " << tx->get_binary_rate() << std::endl;
}
void Map::optimize_placement(int nbr_ant)
{
    if (display == nullptr) {
        std::cout << "No window given to show the data rate" << std::endl;
        return;
    }
    std::cout << "Starting optimization..." << std::endl;
    auto start_time = std::chrono::high_resolution_clock::now();

    setup_accessible_tiles();
    floatVect coverage;
    floatVect total_rate_GB;
    calculate_data_rate(&coverage, &total_rate_GB);
    int max_index = 0;
    for (int i = 0; i < static_cast<int>(coverage.size()); i++) {
        if ((coverage[i] > coverage[max_index]) || ((coverage[i] == coverage[max_index]) && (total_rate_GB[i] >= total_rate_GB[max_index]))) {
            max_index = i;
        }
    }
    std::cout << "Optimal antenna position : ";
    accessible_tiles[max_index]->get_pos().show();
    calculate_data_rate(accessible_tiles[max_index]);
    show_map(display);
    display->add_tiles(0, tiles);

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    std::cout << "Duration : " << duration.count() / 1e6f << " seconds" << std::endl;
}
void Map::show_map(Graphics* display) const
{
    for (Wall* w : walls) {
        display->add_wall(w);
    }
    for (corner* c : corners) {
        display->add_corner(c);
    }
}
void Map::add_wall(Wall* w) {
    walls.push_back(w);
}
void Map::add_corner(corner* c) {
    corners.push_back(c);
}
void Map::setup_materials() {
    exo_4_1 = new Material(4.8f, 0.018f, color({ 168, 168, 160, 255 }), 0.15f);

    air = new Material(1.0f, 0.0f, 0.0f);
    concrete = new Material(6.4954f, 1.43f, color({69, 69, 61, 255}), 0.3f);
    gyproc = new Material(2.7f, 0.05346f, color({168, 168, 160, 255}), 0.1f);
    glass = new Material(6.3919f, 0.00107f, color({126, 235, 230, 200}), 0.05f);
    metal = new Material(1.0f, 1e7f, color({117, 117, 117, 255}), 0.05f);
}
void Map::setup_walls(bool lift) {
    
    // clearing old list
    walls.clear();

    if (EXERCISE) {
        // exo_4_1
        add_wall(new Wall(Vector(0, 0), Vector(0, 1), floatVect{ 0.0f, 80.0f }, exo_4_1));
        add_wall(new Wall(Vector(0, 60), Vector(1, 0), floatVect{ 0.0f, 50.0f }, exo_4_1));
        add_wall(new Wall(Vector(0, 0), Vector(1, 0), floatVect{ 0.0f, 50.0f }, exo_4_1));
        add_corner(new corner(Vector(0, 0), exo_4_1));
    }
    else {
        // composite
        add_wall(new Wall(Vector(4, 0), Vector(0, 1), floatVect{ 0.0f, 4.0f, 6.0f, 8.0f }, matVect{ gyproc, concrete }));
        // concrete
        add_wall(new Wall(Vector(0, 0), Vector(1, 0), floatVect{ 0.0f, 15.0f }, concrete));
        add_wall(new Wall(Vector(0, 0), Vector(0, 1), floatVect{ 0.0f, 8.0f }, concrete));
        add_corner(new corner(Vector(0, 0), concrete));
        add_wall(new Wall(Vector(7, 0), Vector(0, 1), floatVect{ 0.0f, 4.0f }, concrete));
        add_wall(new Wall(Vector(0, 8), Vector(1, 0), floatVect{ 0.0f, 6.0f, 9.0f, 12.0f }, matVect{ concrete, concrete }));
        add_corner(new corner(Vector(0, 8), concrete));
        add_wall(new Wall(Vector(4, 6), Vector(1, 0), floatVect{ 0.0f, 5.0f }, concrete));
        add_corner(new corner(Vector(4, 6), concrete));
        add_wall(new Wall(Vector(9, 6), Vector(0, 1), floatVect{ 0.0f, 2.0f }, concrete));
        add_corner(new corner(Vector(9, 6), concrete));
        add_corner(new corner(Vector(9, 8), concrete));
        add_wall(new Wall(Vector(15, 0), Vector(0, 1), floatVect{ 0.0f, 4.0f }, concrete));
        add_corner(new corner(Vector(15, 0), concrete));
        // gyproc
        add_wall(new Wall(Vector(0, 5), Vector(1, 0), floatVect{ 0.0f, 4.0f }, gyproc));
        add_wall(new Wall(Vector(4, 4), Vector(1, 0), floatVect{ 0.0f, 1.0f, 2.0f, 5.0f, 6.0f, 7.0f }, matVect{ gyproc , gyproc , gyproc }));
        add_corner(new corner(Vector(4, 4), gyproc));
        add_wall(new Wall(Vector(11, 0), Vector(0, 1), floatVect{ 0.0f, 4.0f }, gyproc));
        add_corner(new corner(Vector(11, 4), gyproc));
        // glass
        add_wall(new Wall(Vector(12, 8), Vector(3, -4), floatVect{ 0.0f, 5.0f }, glass));
        // metal
        add_wall(new Wall(Vector(5.85f, 6.0f), Vector(0, 1), floatVect{ 0.0f, 2.0f }, metal));
        if (lift) {
            add_wall(new Wall(Vector(4.25f, 6.25f), Vector(1, 0), floatVect{ 0.0f, 1.5f }, metal));
            add_wall(new Wall(Vector(4.25f, 7.75f), Vector(1, 0), floatVect{ 0.0f, 1.5f }, metal));
            add_wall(new Wall(Vector(4.25f, 6.25f), Vector(0, 1), floatVect{ 0.0f, 1.5f }, metal));
            add_wall(new Wall(Vector(5.75f, 6.25f), Vector(0, 1), floatVect{ 0.0f, 1.5f }, metal));
            add_corner(new corner(Vector(4.25f, 6.25f), metal));
            add_corner(new corner(Vector(4.25f, 7.75f), metal));
            add_corner(new corner(Vector(5.75f, 6.25f), metal));
            add_corner(new corner(Vector(5.75f, 7.75f), metal));
        }
    }
}
void Map::virtualize_antenna(RealAntenna* a) {
    for (int i = 0; i < static_cast<int>(walls.size()); i++) {
        a->virtualize(walls[i]);
    }
}
void Map::create_rays() {
    // direct link
    tx->create_ray(rx, walls);
    // 1 reflexion
    for (Antenna* a : rx->get_virtual_network()) {
        tx->create_ray(a, walls);
    }
    // 2 reflexions
    for (Antenna* a : tx->get_virtual_network()) {
        for (Antenna* b : rx->get_virtual_network()) {
            a->create_ray(b, walls);
        }
    }
}
void Map::calculate_data_rate(floatVect* cov, floatVect* data)
{
    for (int i = 0; i < static_cast<int>(accessible_tiles.size()); i++) {
        tx = accessible_tiles[i]->get_antenna();
        int tiles_covered = 0;
        float total_data = 0;
        for (int j = 0; j < static_cast<int>(accessible_tiles.size()); j++) {
            bool skip = false;
            for (int k = 0; k < i; k++) {
                if (accessible_tiles[k] == accessible_tiles[j]) {
                    // if we already calculated the situation but int the other side : RX <=> TX
                    accessible_tiles[j]->add_rate(accessible_tiles[k]->get_rate(j));
                    skip = true;
                }
            }
            if (!skip) {
                rx = accessible_tiles[j]->get_antenna();
                if (rx == tx) {
                    // if it's the same tile
                    accessible_tiles[j]->add_rate(40e9f);
                }
                else {
                    create_rays();
                    float rate = tx->get_binary_rate();
                    accessible_tiles[j]->add_rate(rate);
                }
            }
            tx->reset();
            total_data += static_cast<float>(accessible_tiles[j]->get_rate(i)/1e9);
            if (accessible_tiles[j]->get_rate(i) != 0) {
                tiles_covered++;
            }
        }
        cov->push_back(static_cast<float>(tiles_covered) / static_cast<float>(accessible_tiles.size()));
        data->push_back(total_data);
        tx = nullptr;
        rx = nullptr;
    }
}
void Map::calculate_data_rate(Tile* tx_tile) {
    tx = tx_tile->get_antenna();
    for (Tile* t : tiles) {
        rx = t->get_antenna();
        if ((rx->get_pos() - tx->get_pos()).squared_norm() < 1e-5f) {
            // if it's the same tile
            t->add_rate(40e9f);
        }
        else {
            create_rays();
            float rate = tx->get_binary_rate();
            t->add_rate(rate);
        }
        tx->reset();
        rx->reset();
    }
    tx = nullptr;
    rx = nullptr;
}
void Map::setup_tiles()
{
    int size[2];
    if (EXERCISE) {
        size[0] = 60;
        size[1] = 80;
    }
    else {
        size[0] = 15;
        size[1] = 8;
    }
    for (int i = 0; i <= static_cast<int>(size[0]/TILE_SIZE); i++) {
        for (int j = 0; j <= static_cast<int>(size[1]/TILE_SIZE); j++) {
            if (!EXERCISE) {
                if (j > static_cast<int>(-4.0/3.0 * i + 24.0/TILE_SIZE)) {
                    continue;
                }
                if ((j > static_cast<int>(6 / TILE_SIZE)) and (i > static_cast<int>(4 / TILE_SIZE)) and (i < static_cast<int>(9 / TILE_SIZE))) {
                    continue;
                }
            }
            Tile* new_tile = new Tile(Vector(i * TILE_SIZE, j * TILE_SIZE));
            virtualize_antenna(new_tile->get_antenna());
            tiles.push_back(new_tile);
        }
    }
}

Tile* Map::find_closest_tile(const Vector& pos) const
{
    Tile* closest_tile = nullptr;
    float distance = 999999.9f;
    for (Tile* t : tiles) {
        if (distance >= (pos - t->get_pos()).squared_norm()) {
            distance = (pos - t->get_pos()).squared_norm();
            closest_tile = t;
        }
    }
    return closest_tile;
}

void Map::setup_accessible_tiles()
{
    for (Tile* t : tiles) {
        if ((t->get_pos().get_x() < 4 || t->get_pos().get_x() > 11 || t->get_pos().get_y() > 4)) {
            // if we are not in the kitchen or in the bathroom
            bool to_keep = true;
            for (Wall* w : walls) {
                if (t->in_wall(w)) {
                    to_keep = false;
                    break;
                }
            }
            if (to_keep) {
                accessible_tiles.push_back(new Tile(t));
            }
        }
    }
}
/* Memory leak ? Depuis que j'ai remplacé les tiles par des new tiles avec un constructeur de copie, le code va 50 fois plus vite (42s -> 0.8s) */
