#pragma once

#include "antenna.h"
#include <map>

class Tile;

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
	float get_rate(int position) const;
	RealAntenna* get_antenna() const; 
	const Vector get_pos() const;

	// ---------- METHODS ----------

public :
	void add_rate(float rate);

	// ---------- ATTRIBUTES ----------

private :
	const Vector pos;
	floatVect rates;
	RealAntenna* antenna;

};

