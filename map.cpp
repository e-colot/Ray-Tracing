#include "map.h"
#include <chrono>
#include <iostream>
#include <stdexcept>

#include <thread>

#define MAX(a, b) ((a > b) ? a : b)

// Constructors

Map::Map(bool lift) : lift(lift) {
	setup_materials();
	setup_walls();
}
Map::Map(Graphics* g, bool lift) : Map(lift) {
	display = g;
}

// Destructor

Map::~Map() {
	display = nullptr;
	delete exo_4_1;
	delete concrete;
	delete gyproc;
	delete glass;
	delete metal;
	for (const Wall* i : walls) {
		delete i;
	}
	for (const corner* i : corners) {
		delete i;
	}
	for (Tile* t : tiles) {
		delete t;
	}
}

// Methods

void Map::show_rays(Vector tx_pos, Vector rx_pos, bool logarithmic) {
	if (display == nullptr) {
		throw std::logic_error("No window given to show the rays");
	}
	RealAntenna* rx = new RealAntenna(rx_pos);
	RealAntenna* tx = new RealAntenna(tx_pos);
	virtualize_antenna(rx);
	virtualize_antenna(tx);
	create_rays(tx, rx);
	show_map();
	display->add_rays(tx, logarithmic);
	for (const Ray* r : tx->get_rays()) {
		r->show();
		std::cout << std::endl;
	}
	delete rx; rx = nullptr;
	delete tx; tx = nullptr;
}
void Map::show_data_rate(const vectorVect& antenna_pos, bool dBm, float tile_size) {
	if (EXERCISE) {
		throw std::logic_error("Cannot show tiles outside of the appartment");
	}
	setup_tiles(tile_size, false);
	if (display == nullptr) {
		throw std::logic_error("No window given to show the data rate");
	}
	display->set_tile_size(tile_size);
	realantennaVect antennas;
	for (Vector pos : antenna_pos) {
		RealAntenna* ant = new RealAntenna(pos);
		virtualize_antenna(ant);
		antennas.push_back(ant);
	}
	calculate_data_rate(antennas);
	for (RealAntenna* ant : antennas) {
		delete ant;
	}
	antennas.clear();
	show_map();
	display->add_tiles(tiles, dBm);
}
void Map::optimize_placement(int number_of_antenna, float precision, char precision_level, bool dBm) {
	if (number_of_antenna <= 0 || number_of_antenna > 2) {
		throw std::logic_error("Optimization with unvalid number of antenna");
	}
	if (precision <= 0) {
		throw std::logic_error("Optimization precision must be positive");
	}
	if (EXERCISE) {
		throw std::logic_error("Cannot show tiles outside of the appartment");
	}
	if (display == nullptr) {
		throw std::logic_error("No window given to optimize");
	}
	float BRUT_FORCE_TILE_SIZE;
	float GRADIENT_DESCENT_TILE_SIZE;
	float DISPLAY_TILE_SIZE;
	switch (precision_level) // 0 = fast, 1 = intermediate, 2 = precise
	{
	case (0):
		BRUT_FORCE_TILE_SIZE = 2.0f;
		GRADIENT_DESCENT_TILE_SIZE = 1.0f;
		DISPLAY_TILE_SIZE = 0.5f;
		break;
	case (1):
		BRUT_FORCE_TILE_SIZE = 1.0f;
		GRADIENT_DESCENT_TILE_SIZE = 0.5f;
		DISPLAY_TILE_SIZE = 0.2f;
		break;
	case (2):
		BRUT_FORCE_TILE_SIZE = 0.5f;
		GRADIENT_DESCENT_TILE_SIZE = 0.2f;
		DISPLAY_TILE_SIZE = 0.1f;
		break;
	default:
		throw std::logic_error("Optimization precision level not acceptable");
	}
	std::cout << "Starting optimization..." << std::endl;
	auto start_time = std::chrono::high_resolution_clock::now();
	std::cout << std::endl << "    Brut force :" << std::endl << std::endl;
	Map brut_force_map = Map(lift);
	vectorVect ant_pos = brut_force_map.brut_force(number_of_antenna, BRUT_FORCE_TILE_SIZE);
	std::cout << std::endl << "    Gradient descent" << std::endl << std::endl;
	Map gradient_descent_map = Map(lift);
	gradient_descent_map.gradient_descent(&ant_pos, GRADIENT_DESCENT_TILE_SIZE, precision);
	std::cout << std::endl << "Optimized router position(s) :" << std::endl;
	for (Vector pos : ant_pos) {
		pos.show();
	}
	show_data_rate(ant_pos, dBm, DISPLAY_TILE_SIZE);

	auto end_time = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
	std::cout << std::endl << "Duration : " << duration.count() / 1e6f << " seconds" << std::endl;
}
vectorVect Map::brut_force(int antenna_number, float tile_size) {
	setup_tiles(tile_size, true);
	calculate_data_rate();
	return best_position(antenna_number);
}
void Map::gradient_descent(vectorVect* pos, float tile_size, float precision) {
	setup_tiles(tile_size, true);
	bool searching = true;
	realantennaVect antennas;
	floatVect actual;
	floatVect best;
	while (searching) {
		searching = false;
		for (int i = 0; i < static_cast<int>((*pos).size()); i++) {
			for (RealAntenna* a : antennas) {
				delete a;
			}
			antennas.clear();
			for (Vector p : *pos) {
				antennas.push_back(new RealAntenna(p));
			}
			for (Vector dir : {Vector(1, 0), Vector(0, 1)}) {
				// chosing the direction (left/right or up/down)
				for (float value : {-precision, 0.0f, precision}) {
					delete antennas[i];
					antennas[i] = new RealAntenna(pos->at(i) + dir*value);
					calculate_data_rate(antennas);
				}
				int val = best_of_3(&best);
				if (val != 0) {
					searching = true;
				}
				// while going in this direction is better, goes in this direction
				bool better = true;
				while (better && val != 0) {
					pos->at(i) = pos->at(i) + dir * val * precision;
					delete antennas[i];
					antennas[i] = new RealAntenna(pos->at(i));
					calculate_data_rate(antennas);
					better = best_of_2(&best);
				}
			}
		}
		if (searching) {
			for (Vector p : *pos) {
				p.show();
			}
			std::cout << std::endl;
		}
	}
	for (RealAntenna* a : antennas) {
		delete a;
	}
	std::cout << "Coverage after gradient descent : " << 100*best[0]/tiles.size() << "%" << std::endl;
}
int Map::best_of_3(floatVect* best) const { // should only be called from gradient descent
	int best_index = 0;
	int best_coverage = 0;
	float best_data = 0;
	for (int i : {0, 1, 2}) {
		int cov = 0;
		float data = 0.0f;
		for (Tile* t : tiles) {
			if (t->get_rate(i) > std::numeric_limits<double>::epsilon()) {
				cov++;
				data += static_cast<float>(t->get_rate(i));
			}
		}
		if ((cov > best_coverage) || (cov >= best_coverage && data > best_data + std::numeric_limits<float>::epsilon())) {
			best_index = i;
			best_coverage = cov;
			best_data = data;
		}
	}
	*best = { static_cast<float>(best_coverage), best_data };
	// clear the rates for each tile (to call the function again)
	for (Tile* t : tiles) {
		t->delete_rates();
	}
	return (best_index - 1); // -1 to get the direction (in accordance to Map::gradient_descent)
}
bool Map::best_of_2(floatVect* best) const {
	int cov = 0;
	float data = 0.0f;
	for (Tile* t : tiles) {
		if (t->get_rate(0) > std::numeric_limits<double>::epsilon()) {
			cov++;
			data += static_cast<float>(t->get_rate(0));
		}
	}
	for (Tile* t : tiles) {
		t->delete_rates();
	}
	if ((cov > best->at(0) || (cov >= best->at(0) && data > (best->at(1) + std::numeric_limits<float>::epsilon())))) {
		*best = { static_cast<float>(cov), data };
		return true;
	}
	return false;
}
void Map::show_map() const {
	for (const Wall* w : walls) {
		display->add_wall(w);
	}
	for (const corner* c : corners) {
		display->add_corner(c);
	}
}
void Map::add_wall(const Wall* w) {
	walls.push_back(w);
}
void Map::add_corner(const corner* c) {
	corners.push_back(c);
}
void Map::setup_materials() {
	if (EXERCISE) {
		exo_4_1 = new Material(4.8f, 0.018f, color({ 168, 168, 160, 255 }), 0.15f);
	}
	else {
		concrete = new Material(6.4954f, 1.43f, color({ 69, 69, 61, 255 }), 0.3f);
		gyproc = new Material(2.7f, 0.05346f, color({ 168, 168, 160, 255 }), 0.1f);
		glass = new Material(6.3919f, 0.00107f, color({ 126, 235, 230, 200 }), 0.05f);
		metal = new Material(1.0f, 1e7f, color({ 117, 117, 117, 255 }), 0.05f);
	}
}
void Map::setup_walls() {
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
		add_wall(new Wall(Vector(4, 4), Vector(1, 0), floatVect{ 0.0f, 1.0f, 2.0f, 5.0f, 6.0f, 7.0f }, matVect{ gyproc , gyproc , gyproc }));
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
		add_wall(new Wall(Vector(4, 0), Vector(0, 1), floatVect{ 0.0f, 4.0f, 6.0f, 8.0f }, matVect{ gyproc, concrete }));
		add_wall(new Wall(Vector(0, 5), Vector(1, 0), floatVect{ 0.0f, 4.0f }, gyproc));
		add_corner(new corner(Vector(4, 4), gyproc));
		add_wall(new Wall(Vector(11, 0), Vector(0, 1), floatVect{ 0.0f, 4.0f }, gyproc));
		add_corner(new corner(Vector(11, 4), gyproc));
		add_wall(new Wall(Vector(12, 8), Vector(3, -4), floatVect{ 0.0f, 5.0f }, glass));
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
void Map::virtualize_antenna(RealAntenna* a) const {
	for (const Wall* w : walls) {
		a->virtualize(w);
	}
}

void Map::create_rays(RealAntenna* tx, const RealAntenna* rx) const {
	// direct link
	tx->create_ray(rx, walls);

	// 1 reflexion
	for (int i = 0; i < static_cast<int>(rx->get_virtual_network().size()); i++) {
		tx->create_ray(rx->get_virtual_network()[i], walls);
	}

	// 2 reflexions
	for (int i = 0; i < static_cast<int>(tx->get_virtual_network().size()); i++) {
		for (int j = 0; j < static_cast<int>(rx->get_virtual_network().size()); j++) {
			tx->get_virtual_network()[i]->create_ray(rx->get_virtual_network()[j], walls);
		}
	}
	tx->calc_attenuation();
}
void Map::calculate_data_rate() {
	RealAntenna* tx = nullptr;
	RealAntenna* rx = nullptr;
	for (int i = 0; i < static_cast<int>(tiles.size()); i++) {
		tx = tiles[i]->get_antenna();
		for (int j = 0; j < static_cast<int>(tiles.size()); j++) {
			if (j < i) {
				// if we already calculated the situation but in the other side : RX <=> TX
				tiles[j]->add_rate(tiles[i]->get_rate(j)); //(50% time saved)
			}
			else if ((tx->get_pos().get_x() > 4 && tx->get_pos().get_x() < 11 && tx->get_pos().get_y() < 4) && 
				((tiles[j]->get_pos().get_x() > 4 && tiles[j]->get_pos().get_x() < 11 && tiles[j]->get_pos().get_y() < 4))) {
				// if both rx and tx are in the kitchen or the bathroom (ensures that putting the router in one of those rooms has a huge prejudice)
				tiles[j]->add_rate(0.0); // (10% time saved)
			}
			else {
				rx = tiles[j]->get_antenna();
				tiles[j]->add_rate(calc_rate(tx, rx));
			}
		}
		tx = nullptr;
		rx = nullptr;
	}
}
void Map::calculate_data_rate(const realantennaVect& tx_antenna) {
	floatVect data_rate_values;
	RealAntenna* tx = nullptr;
	RealAntenna* rx = nullptr;
	for (int k = 0; k < static_cast<int>(tx_antenna.size()); k++) {
		tx = tx_antenna[k];
		for (int i = 0; i < static_cast<int>(tiles.size()); i++) {
			rx = tiles[i]->get_antenna();
			data_rate_values.push_back(static_cast<float>(calc_rate(tx, rx))); // not a final result so losing some precision is acceptable
		}
		tx = nullptr;
		rx = nullptr;
	}
	int size = static_cast<int>(tiles.size());
	for (int i = 0; i < size; i++) {
		float data = 0.0f;
		for (int j = 0; j < static_cast<int>(tx_antenna.size()); j++) {
			data = MAX(data, data_rate_values[i + j * size]);
		}
		tiles[i]->add_rate(data);
	}
}
void Map::setup_tiles(float tile_size, bool restrained) {
	if (EXERCISE) {
		throw std::logic_error("Cannot calculate tiles outside of the appartment");
	}
	// deleting tiles
	for (Tile* t : tiles) {
		delete t;
	}
	tiles.clear();
	int size[2]{15, 8};
	for (int i = 0; i <= static_cast<int>(size[0] / tile_size); i++) {
		for (int j = 0; j <= static_cast<int>(size[1] / tile_size); j++) {
			if (!lift) {
				// if there is no lift, we don't calculate the tiles outside of the appartment
				if (static_cast<float>(j) > (-4.0 / 3.0 * i + 24.0 / tile_size)) {
					continue;
				}
				if ((static_cast<float>(j) > (6 / tile_size)) and (static_cast<float>(i) > (4 / tile_size)) and (static_cast<float>(i) < (9 / tile_size))) {
					continue;
				}
			}
			if (restrained) {
				// using only some tiles for the optimisation saves 75% time for the brut force part (with 0.5 x 0.5 tiles)
				bool to_keep = true;
				for (const Wall* w : walls) {
					if (w->inside(Vector(i * tile_size, j * tile_size))) {
						to_keep = false;
						break;
					}
				}
				if (!to_keep) {
					continue;
				}
			}
			Tile* new_tile = new Tile(Vector(i * tile_size, j * tile_size));
			virtualize_antenna(new_tile->get_antenna());
			tiles.push_back(new_tile);
		}
	}
}
double Map::calc_rate(RealAntenna* tx, const RealAntenna* rx) const { // only called from calculate_data_rate
	double output;
	if (tx->get_pos() == rx->get_pos()) {
		// if the emitter are close enough
		output = 4e10f;
	}
	else {
		create_rays(tx, rx);
		output = tx->get_binary_rate();
	}
	tx->reset();
	return output;
}
vectorVect Map::best_position(int nbr_antennas) const { // only called from brutforce
	if (nbr_antennas > 2) {
		throw std::logic_error("More than 2 antennas not handled (yet ?)");
	}
	vectorVect res;
	if (nbr_antennas == 1) {
		floatVect coverage;
		floatVect total_rate_GB;
		int max_index = 0;
		// the following finds the coverage and the total data rate of each antenna
		for (int i = 0; i < static_cast<int>(tiles.size()); i++) {
			int cov = 0;
			float data = 0;
			for (Tile* t : tiles) {
				if (t->get_rate(i) > std::numeric_limits<double>::epsilon()) {
					// if rate != 0
					cov++;
					data += (static_cast<float>(t->get_rate(i)) / 1e9f);
				}
			}
			total_rate_GB.push_back(data);
			coverage.push_back(static_cast<float>(cov) / static_cast<float>(tiles.size()));
			if ((coverage[i] > coverage[max_index]) || ((coverage[i] >= coverage[max_index] - std::numeric_limits<double>::epsilon()) &&
				(total_rate_GB[i] >= total_rate_GB[max_index] - std::numeric_limits<double>::epsilon()))) {
				max_index = i;
			}
		}
		res.push_back(tiles[max_index]->get_pos());
		for (Vector pos : res) {
			pos.show();
		}
		std::cout << std::endl << "Approached coverage after brut force : " << 100 * coverage[max_index] << "%" << std::endl;
	}
	else if (nbr_antennas == 2) {
		floatMatrix coverage;
		floatMatrix total_rate_GB;
		int i_antenna = 0;
		int j_antenna = 0;

		// the following finds the coverage and the total data rate of each pair of antenna
		for (int i = 0; i < static_cast<int>(tiles.size()); i++) {
			floatVect cover;
			floatVect data_rate;
			for (int j = 0; j < static_cast<int>(tiles.size()); j++) {
				if (j < i) {
					cover.push_back(coverage[j][i]);
					data_rate.push_back(total_rate_GB[j][i]);
				}
				else {
					int cov = 0;
					float data = 0;
					for (Tile* t : tiles) {
						if (t->get_rate(i) > std::numeric_limits<double>::epsilon() || t->get_rate(j) > std::numeric_limits<double>::epsilon()) {
							cov++;
							data += static_cast<float>(static_cast<float>(MAX(t->get_rate(i), t->get_rate(j))) / 1e9f);
						}
					}
					cover.push_back(static_cast<float>(cov) / static_cast<float>(tiles.size()));
					data_rate.push_back(data);
				}
			}
			coverage.push_back(cover);
			total_rate_GB.push_back(data_rate);

			for (int j = 0; j < static_cast<int>(tiles.size()); j++) {
				if ((coverage[i][j] > coverage[i_antenna][j_antenna]) || ((coverage[i][j] >= coverage[i_antenna][j_antenna] - std::numeric_limits<double>::epsilon()) &&
					(total_rate_GB[i][j] >= total_rate_GB[i_antenna][j_antenna] - std::numeric_limits<double>::epsilon()))) {
					i_antenna = i;
					j_antenna = j;
				}
			}
		}
		res.push_back(tiles[i_antenna]->get_pos());
		res.push_back(tiles[j_antenna]->get_pos());
		for (Vector pos : res) {
			pos.show();
		}
		std::cout << std::endl << "Approached coverage after brut force : " << 100*coverage[i_antenna][j_antenna] << "%" << std::endl;
	}
	// more antennas could be handled using the same kind of code but with higher dimensions vectors
	return res;
}
