#pragma once
#include <SDL_types.h> // Include the header for Uint8 type
#include <initializer_list> // Include the header for std::initializer_list

extern  bool EXERCISE;
extern const float MU_0;
extern const float EPS_0;
extern  long long FREQUENCY;
extern const float PI;
extern  long long PULS;
extern const int C;
extern const float Z_0;
extern const int R_A;
extern float P_TX;
extern float BETA_AIR;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern int SCALE;

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

void CHANGE_TYPE(bool exercise);
