#pragma once
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-default"
#include <SDL_types.h> // Include the header for Uint8 type
#pragma GCC diagnostic pop
#include <initializer_list> // Include the header for std::initializer_list

extern const bool EXERCISE;
extern const float MU_0;
extern const float EPS_0;
extern const long long FREQUENCY;
extern const float PI;
extern const long long PULS;
extern const int C;
extern const float Z_0;
extern const int R_A;
extern const float P_TX;
extern const float BETA_AIR;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int SCALE;
extern float TILE_SIZE;

struct color {
	// the line above allows to initialize a color without storing a list before creating the color
	Uint8 r; // red
	Uint8 g; // green
	Uint8 b; // blue
	Uint8 a; // alpha, opacity
	color() : r(0), g(0), b(0), a(255) {}
	color(Uint8 val[4]) : r(val[0]), g(val[1]), b(val[2]), a(val[3]) {}
	color(std::initializer_list<Uint8> init) : r(*(init.begin() + 0)), g(*(init.begin() + 1)), b(*(init.begin() + 2)), a(*(init.begin() + 3)) {}
};
