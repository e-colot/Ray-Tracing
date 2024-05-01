#pragma once

#include "antenna.h"
#include <map>

class Tile;

using doubleVect = std::vector<double>;

class Tile
{

	// ---------- CONSTRUCTORS ----------

public :
	Tile();
	Tile(const Vector& center_of_tile);
	Tile(const Tile* origin);

	// ---------- DESTRUCTORS ----------

public: 
	~Tile();

	// ---------- ACCESSORS ----------

public :
	double get_rate(int position) const;
	RealAntenna* get_antenna() const; 
	const Vector get_pos() const;

	// ---------- METHODS ----------

public :
	void add_rate(double rate);

	// ---------- ATTRIBUTES ----------

private :
	const Vector pos;
	doubleVect rates;
	RealAntenna* antenna;

};

