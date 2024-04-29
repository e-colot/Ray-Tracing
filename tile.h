#pragma once

#include "antenna.h"
#include <map>

class Tile;

class Tile
{

	// ---------- CONSTRUCTORS ----------

public :
	Tile();
	Tile(Vector center_of_tile);
	Tile(Tile* origin);

	// ---------- DESTRUCTORS ----------

public: 
	~Tile();

	// ---------- ACCESSORS ----------

public :
	float get_rate(int position);
	RealAntenna* get_antenna() const; 
	Vector get_pos() const;

	// ---------- METHODS ----------

public :
	void add_rate(float rate);
	bool in_wall(Wall* wall) const;

	// ---------- ATTRIBUTES ----------

private :
	Vector pos;
	floatVect rates;
	RealAntenna* antenna;

};

