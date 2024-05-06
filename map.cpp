#include "map.h"
#include <chrono>
#include <iostream>

#define MAX(a, b) ((a > b) ? a : b)

// Constructors

Map::Map() : exo_4_1(nullptr), concrete(nullptr), gyproc(nullptr),
glass(nullptr), metal(nullptr), display(nullptr), rx(nullptr), tx(nullptr) {}
Map::Map(Graphics* g) {
	setup_materials();
	setup_walls(false);
	add_window(g);
}
Map::Map(const Vector& tx_pos, const Vector& rx_pos) : display(nullptr) {
	setup_materials();
	setup_walls(false);
	tx = new RealAntenna(tx_pos);
	rx = new RealAntenna(rx_pos);
}
Map::Map(const Vector& tx_pos, const Vector& rx_pos, Graphics* g) : Map(tx_pos, rx_pos) {
	add_window(g);
}

// Destructor

Map::~Map() {
	display = nullptr;
	rx = nullptr;
	tx = nullptr;
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

// Mutator

void Map::add_window(Graphics* g) {
	display = g;
}

// Methods

void Map::show_rays(bool logarithmic) const {
	if (display == nullptr) {
		std::cout << "No window given to show the rays" << std::endl;
		return;
	}
	virtualize_antenna(rx);
	virtualize_antenna(tx);
	create_rays();
	show_map();
	display->add_rays(tx, logarithmic);
	for (const Ray* r : tx->get_rays()) {
		r->show();
		std::cout << std::endl;
	}
}
void Map::show_data_rate(const vectorVect& antenna_pos, bool dBm, float tile_size) {
	setup_tiles(tile_size);
	if (display == nullptr) {
		std::cout << "No window given to show the data rate" << std::endl;
		return;
	}
	tileVect these_tiles;
	for (Vector pos : antenna_pos) {
		these_tiles.push_back(find_closest_tile(pos));
	}
	calculate_data_rate(these_tiles);
	show_map();
	display->add_tiles(tiles, dBm);
}
void Map::optimize_placement(int number_of_antenna)
{
	std::cout << "Starting optimization..." << std::endl;
	auto start_time = std::chrono::high_resolution_clock::now();

	vectorVect ant_pos = brut_force(number_of_antenna, 2.0f);
	std::cout << "Gradient descent" << std::endl;
	gradient_descent(&ant_pos, 0.5f, 0.1f);
	realantennaVect final_antennas;
	std::cout << "Optimized router positions :" << std::endl;
	for (Vector pos : ant_pos) {
		pos.show();
	}
	show_data_rate(ant_pos);

	auto end_time = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
	std::cout << "Duration : " << duration.count() / 1e6f << " seconds" << std::endl;
}
vectorVect Map::brut_force(int antenna_number, float tile_size) {
	if (display == nullptr) {
		std::cout << "No window given to show the data rate" << std::endl;
		return vectorVect{};
	}
	vectorVect res;
	setup_tiles(tile_size);
	setup_accessible_tiles();
	calculate_data_rate();
	if (antenna_number == 1) {
		floatVect coverage;
		floatVect total_rate_GB;
		int max_index = 0;
		// the following finds the coverage and the total data rate of each antenna
		for (int i = 0; i < static_cast<int>(accessible_tiles.size()); i++) {
			int cov = 0;
			float data = 0;
			for (Tile* t : accessible_tiles) {
				if (t->get_rate(i) > std::numeric_limits<double>::epsilon()) {
					// if rate != 0
					cov++;
					data += (static_cast<float>(t->get_rate(i)) / 1e9f);
				}
			}
			total_rate_GB.push_back(data);
			coverage.push_back(static_cast<float>(cov) / static_cast<float>(accessible_tiles.size()));
			if ((coverage[i] > coverage[max_index]) || ((coverage[i] >= coverage[max_index] - std::numeric_limits<double>::epsilon()) && 
					(total_rate_GB[i] >= total_rate_GB[max_index] - std::numeric_limits<double>::epsilon()))) {
				max_index = i;
			}
		}

		//std::cout << "Optimal antenna position : ";
		//accessible_tiles[max_index]->get_pos().show();
		//std::cout << "Coverage : " << coverage[max_index] * 100.0 << "%" << std::endl;
		res.push_back(accessible_tiles[max_index]->get_pos());
		//calculate_data_rate(accessible_tiles[max_index]);
		//show_map();
		//display->add_tiles(tiles);
	}
	else if (antenna_number == 2) {
		floatMatrix coverage;
		floatMatrix total_rate_GB;
		int i_antenna = 0;
		int j_antenna = 0;

		// the following finds the coverage and the total data rate of each pair of antenna
		for (int i = 0; i < static_cast<int>(accessible_tiles.size()); i++) {
			floatVect cover;
			floatVect data_rate;
			for (int j = 0; j < static_cast<int>(accessible_tiles.size()); j++) {
				if (j < i) {
					cover.push_back(coverage[j][i]);
					data_rate.push_back(total_rate_GB[j][i]);
				}
				else {
					int cov = 0;
					float data = 0;
					for (Tile* t : accessible_tiles) {
						if (t->get_rate(i) > std::numeric_limits<double>::epsilon() || t->get_rate(j) > std::numeric_limits<double>::epsilon()) {
							cov++;
							data += static_cast<float>(static_cast<float>(MAX(t->get_rate(i), t->get_rate(j))) / 1e9f);
						}
					}
					cover.push_back(static_cast<float>(cov) / static_cast<float>(accessible_tiles.size()));
					data_rate.push_back(data);
				}
			}
			coverage.push_back(cover);
			total_rate_GB.push_back(data_rate);

			for (int j = 0; j < static_cast<int>(accessible_tiles.size()); j++) {
				if ((coverage[i][j] > coverage[i_antenna][j_antenna]) || ((coverage[i][j] >= coverage[i_antenna][j_antenna] - std::numeric_limits<double>::epsilon()) && 
						(total_rate_GB[i][j] >= total_rate_GB[i_antenna][j_antenna] - std::numeric_limits<double>::epsilon()))) {
					i_antenna = i;
					j_antenna = j;
				}
			}
		}
		//std::cout << "Coverage : " << coverage[i_antenna][j_antenna] * 100.0 << "%" << std::endl;

		//std::cout << "Optimal positions for 2 antennas : " << std::endl;
		//accessible_tiles[i_antenna]->get_pos().show();
		//accessible_tiles[j_antenna]->get_pos().show();
		//tileVect final_antennas = { new Tile(accessible_tiles[i_antenna]) , new Tile(accessible_tiles[j_antenna]) };
		res.push_back(accessible_tiles[i_antenna]->get_pos());
		res.push_back(accessible_tiles[j_antenna]->get_pos());
		//calculate_data_rate(final_antennas);
		//show_map();
		//display->add_tiles(tiles, false);
		//for (Tile* t : final_antennas) {
		//	delete t;
		//}
	}
	// more antennas could be handled using the same kind of code but with higher dimensions vectors
	return res;
}
void Map::gradient_descent(vectorVect* pos, float tile_size, float precision) {
	setup_tiles(tile_size);
	RealAntenna* E = nullptr;
	RealAntenna* W = nullptr;
	RealAntenna* N = nullptr;
	RealAntenna* S = nullptr;
	RealAntenna* C = nullptr;
	realantennaVect antennas;
	for (Vector p : *pos) {
		antennas.push_back(new RealAntenna(p)); // the few antennas created here are never deleted ?
	}
	for (RealAntenna* ant : antennas) {
		virtualize_antenna(ant);
	}
	for (int i = 0; i < static_cast<int>(pos->size()); i++) {
		bool searching = true;
		C = antennas[i];
		while (searching) {
			C->get_pos().show();
			floatMatrix direction_values = floatMatrix{ {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f} };
			direction_values[0] = calculate_coverage_and_rate(antennas);
			// putting a new antenna in each direction
			if (W == nullptr) {
				W = new RealAntenna(pos->at(i) + Vector(-precision, 0.0f));
				virtualize_antenna(W);
				antennas[i] = W;
				direction_values[1] = calculate_coverage_and_rate(antennas);
			}
			if (E == nullptr) {
				E = new RealAntenna(pos->at(i) + Vector(precision, 0.0f));
				virtualize_antenna(E);
				antennas[i] = E;
				direction_values[2] = calculate_coverage_and_rate(antennas);
			}
			if (S == nullptr) {
				S = new RealAntenna(pos->at(i) + Vector(0.0f, -precision));
				virtualize_antenna(S);
				antennas[i] = S;
				direction_values[3] = calculate_coverage_and_rate(antennas);
			}
			if (N == nullptr) {
				N = new RealAntenna(pos->at(i) + Vector(0.0f, precision));
				virtualize_antenna(N);
				antennas[i] = N;
				direction_values[4] = calculate_coverage_and_rate(antennas);
			}
			int best_pos = best_direction(direction_values);
			Vector store_pos;
			switch (best_pos) {
			case(1):
				delete E; 
				direction_values[2] = direction_values[4]; 
				E = N; 
				N = nullptr;
				delete S; 
				direction_values[3] = direction_values[1]; 
				S = W;
				W = nullptr;
				store_pos = C->get_pos();
				delete C; 
				C = new RealAntenna(store_pos + Vector(-precision, +precision));
				virtualize_antenna(C);
				break;
			case(2):
				delete E; E = nullptr;
				delete W; W = nullptr;
				delete S; 
				direction_values[3] = direction_values[0]; 
				S = C;
				direction_values[0] = direction_values[4];
				C = N; N = nullptr; 
				break;
			case(3):
				delete W;
				direction_values[1] = direction_values[4]; 
				W = N; 
				N = nullptr;
				delete S; direction_values[3] = direction_values[2]; 
				S = E; 
				E = nullptr;
				store_pos = C->get_pos();
				delete C; 
				C = new RealAntenna(store_pos + Vector(+precision, +precision));
				virtualize_antenna(C);
				break;
			case(4):
				delete N; 
				N = nullptr;
				delete S; 
				S = nullptr;
				delete E;
				direction_values[2] = direction_values[0]; 
				E = C;
				direction_values[0] = direction_values[1];
				C = W;
				W = nullptr; 
				break;
			case(5):
				delete E; delete W; delete N; delete S;
				E = nullptr; W = nullptr; N = nullptr; S = nullptr;
				antennas[i] = C;
				searching = false; 
				break;
			case(6):
				delete N;
				N = nullptr;
				delete S;
				S = nullptr;
				delete W;
				direction_values[1] = direction_values[0]; 
				W = C;
				direction_values[0] = direction_values[2]; C = E;
				E = nullptr; 
				break;
			case(7):
				delete E; 
				direction_values[2] = direction_values[3]; 
				E = S; 
				S = nullptr;
				delete N; 
				direction_values[4] = direction_values[1]; 
				N = W; 
				W = nullptr;
				store_pos = C->get_pos();
				delete C; 
				C = new RealAntenna(store_pos + Vector(-precision, -precision));
				virtualize_antenna(C);
				break;
			case(8):
				delete E; 
				E = nullptr;
				delete W;
				W = nullptr;
				delete N; 
				direction_values[4] = direction_values[0];
				N = C;
				direction_values[0] = direction_values[3];
				C = S; 
				S = nullptr; 
				break;
			case(9):
				delete W; 
				direction_values[1] = direction_values[3];
				W = S;
				S = nullptr;
				delete N; 
				direction_values[4] = direction_values[2];
				N = E;
				E = nullptr;
				store_pos = C->get_pos();
				delete C;
				C = new RealAntenna(store_pos + Vector(+precision, -precision)); 
				virtualize_antenna(C);
				break;
			default:
				break;
			}
		}
	}
	pos->clear();
	for (RealAntenna* ant : antennas) {
		pos->push_back(ant->get_pos());
		delete ant;
	}
}
int Map::best_direction(const floatMatrix& dir_val) {
	/* returns the placement of the best antenna
	*
						1	2	3
						4	5	6
						7	8	9
	*/
	/*
	best_x stores the best antenna between (W, C, E) = (-1, 0, 1)	(C = center)
	best_y stores the best antenna between (S, C, N) = (-1, 0, 1)	(C = center)
	*/
	intVect best = { 0, 0 }; // = { best_x, best_y }
	for (int j = 0; j <= 1; j++) {
		// for each axis
		for (int i = (2 * j + 1); i <= 2 * (j + 1); i++) {
			if ((dir_val[i][0] > dir_val[best[j]][0]) || (dir_val[i][0] >= dir_val[best[j]][0] && dir_val[i][1] > dir_val[best[j]][1])) {
				best[j] = i;
			}
		}
	}
	if (best[0] == 0) {
		if (best[1] == 0) {
			return 5;
		}
		return 26 - 6 * best[1];
	}
	else if (best[1] == 0) {
		return 2 + 2 * best[0];
	}
	else {
		return 23 + 2 * best[0] - 6 * best[1];
	}
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
	exo_4_1 = new Material(4.8f, 0.018f, color({ 168, 168, 160, 255 }), 0.15f);

	concrete = new Material(6.4954f, 1.43f, color({ 69, 69, 61, 255 }), 0.3f);
	gyproc = new Material(2.7f, 0.05346f, color({ 168, 168, 160, 255 }), 0.1f);
	glass = new Material(6.3919f, 0.00107f, color({ 126, 235, 230, 200 }), 0.05f);
	metal = new Material(1.0f, 1e7f, color({ 117, 117, 117, 255 }), 0.05f);
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
void Map::create_rays() const {
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
void Map::calculate_data_rate() {
	for (int i = 0; i < static_cast<int>(accessible_tiles.size()); i++) {
		tx = accessible_tiles[i]->get_antenna();
		for (int j = 0; j < static_cast<int>(accessible_tiles.size()); j++) {
			if (j < i) {
				// if we already calculated the situation but in the other side : RX <=> TX
				accessible_tiles[j]->add_rate(accessible_tiles[i]->get_rate(j)); //(50% time saved)
			}
			else if ((tx->get_pos().get_x() > 4 && tx->get_pos().get_x() < 11 && tx->get_pos().get_y() < 4) && 
				((accessible_tiles[j]->get_pos().get_x() > 4 && accessible_tiles[j]->get_pos().get_x() < 11 && accessible_tiles[j]->get_pos().get_y() < 4))) {
				// if both rx and tx are in the kitchen or the bathroom
				accessible_tiles[j]->add_rate(0.0); // (10% time saved)
			}
			else {
				rx = accessible_tiles[j]->get_antenna();
				accessible_tiles[j]->add_rate(calc_rate());
			}
		}
		tx = nullptr;
		rx = nullptr;
	}
}
void Map::calculate_data_rate(Tile* tx_tile) {
	clean_accessible_tiles_data();
	tx = tx_tile->get_antenna();
	for (Tile* t : tiles) {
		rx = t->get_antenna();
		t->add_rate(calc_rate());
	}
	tx = nullptr;
	rx = nullptr;
}
void Map::calculate_data_rate(const realantennaVect& tx_antenna)
{
	clean_accessible_tiles_data();
	floatVect data_rate_values;
	for (RealAntenna* tx_ant : tx_antenna) {
		tx = tx_ant;
		for (Tile* t : tiles) {
			rx = t->get_antenna();
			data_rate_values.push_back(static_cast<float>(calc_rate())); // not a final result so losing some precision is acceptable
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
void Map::calculate_data_rate(const tileVect& tx_tiles) {
	realantennaVect antennas;
	for (Tile* t : tx_tiles) {
		antennas.push_back(t->get_antenna());
	}
	calculate_data_rate(antennas);
}
floatVect Map::calculate_coverage_and_rate(const realantennaVect& tx_antenna) {
	// returns (coverage, data_rate)
	floatVect data_rate_values;
	for (RealAntenna* tx_ant : tx_antenna) {
		tx = tx_ant;
		for (Tile* t : tiles) {
			rx = t->get_antenna();
			data_rate_values.push_back(static_cast<float>(calc_rate())); // not a final result so losing some precision is acceptable
		}
		tx = nullptr;
		rx = nullptr;
	}
	float total_data = 0.0f;
	float coverage = 0.0f;
	//int size = static_cast<int>(tx_antenna.size());
	//for (int i = 0; i < size; i++) {
	//	float data = 0.0f;
	//	for (int j = 0; j < static_cast<int>(tiles.size()); j++) {
	//		data = static_cast<float>(MAX(data, data_rate_values[i + j * size]));
	//	}
	//	if (data > 0.0f) {
	//		coverage++;
	//		total_data += data * 1e-9; // in Gb
	//	}
	//}
	for (int i = 0; i < static_cast<int>(tiles.size()); i++) {
		float tile_data = 0.0f;
		for (int j = 0; j < static_cast<int>(tx_antenna.size()); j++) {
			tile_data = MAX(tile_data, data_rate_values[i + j * static_cast<int>(tiles.size())]);
		}
		if (tile_data > 0.0f) {
			coverage++;
			total_data += tile_data * 1e-9; // in Gb
		}
	}
	return floatVect{ coverage, total_data };
}
void Map::setup_tiles(float tile_size) {
	display->set_tile_size(tile_size);
	// deleting tiles
	for (Tile* t : tiles) {
		delete t;
	}
	tiles.clear();
	int size[2]{};
	if (EXERCISE) {
		size[0] = 60;
		size[1] = 80;
	}
	else {
		size[0] = 15;
		size[1] = 8;
	}
	for (int i = 0; i <= static_cast<int>(size[0] / tile_size); i++) {
		for (int j = 0; j <= static_cast<int>(size[1] / tile_size); j++) {
			if (static_cast<float>(j) > (-4.0 / 3.0 * i + 24.0 / tile_size)) {
				continue;
			}
			if ((static_cast<float>(j) > (6 / tile_size)) and (static_cast<float>(i) > (4 / tile_size)) and (static_cast<float>(i) < (9 / tile_size))) {
				continue;
			}
			Tile* new_tile = new Tile(Vector(i * tile_size, j * tile_size));
			virtualize_antenna(new_tile->get_antenna());
			tiles.push_back(new_tile);
		}
	}
}
Tile* Map::find_closest_tile(const Vector& pos) const {
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
void Map::setup_accessible_tiles() {
	// using only some tiles for the optimisation
	// (75% time saved)
	accessible_tiles.clear();
	// deleting accessible_tiles
	for (Tile* t : tiles) {
		bool to_keep = true;
		for (const Wall* w : walls) {
			if (w->inside(t->get_pos())) {
				to_keep = false;
				break;
			}
		}
		if (to_keep) {
			accessible_tiles.push_back(t);
		}
	}
}
double Map::calc_rate() const {
	double output;
	if (tx->get_pos() == rx->get_pos()) {
		// if the emitter are close enough
		output = 4e10f;
	}
	else {
		create_rays();
		output = tx->get_binary_rate();
	}
	tx->reset();
	rx->reset();
	return output;
}
void Map::clean_accessible_tiles_data() const {
	for (Tile* t : accessible_tiles) {
		t->delete_rates();
	}
}
