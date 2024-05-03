#include "tile.h"
#include <iostream>

#define ABS(a) ((a) >= 0? (a) : -(a))

// Constructors
Tile::Tile() : pos(Vector()), antenna(nullptr) {}
Tile::Tile(const Vector& center_of_zone) : pos(center_of_zone) {
    antenna = new RealAntenna(pos);
}
Tile::Tile(const Tile* origin) : Tile(origin->get_pos()) {}

// Destructor
Tile::~Tile() {
    delete antenna;
}

// Accessors
double Tile::get_rate(int i) const {
    return rates[i];
}
RealAntenna* Tile::get_antenna() const {
    return antenna;
}
const Vector& Tile::get_pos() const {
    return pos;
}

// Method
void Tile::add_rate(double rate_in) {
    rates.push_back(rate_in);
}
void Tile::delete_rates() {
    rates.clear();
}
