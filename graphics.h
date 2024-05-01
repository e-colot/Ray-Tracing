#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <vector>
#include "tile.h"

using tileVect = std::vector<Tile*>;

struct colored_rect {
	const SDL_Rect rect;
	const color c;
	colored_rect() : rect{ 0, 0, 0, 0 }, c() {}
	colored_rect(const Vector& start, int length, int width, const color col) : 
		rect({ static_cast<int>(start.get_x() - width / 2.0f), static_cast<int>(start.get_y()), width, length }), c(col) {}
};
struct colored_line {
	const int x1;
	const int y1;
	const int x2;
	const int y2;
	const color c;
	colored_line() : x1(0), y1(0), x2(0), y2(0), c() {}
	colored_line(const Vector& start, const Vector& end, color col) :
		x1(static_cast<int>(start.get_x())), y1(static_cast<int>(start.get_y())), x2(static_cast<int>(end.get_x())), 
		y2(static_cast<int>(end.get_y())), c(col) {}
};
struct corner {
	const Vector pos;
	const Material* mat;
	corner() : pos(Vector()), mat(new Material()) {}
	corner(Vector position, const Material* material) : pos(position), mat(material) {}
};
struct txt {
	SDL_Texture* texture;
	const SDL_Rect rect;
	txt(SDL_Texture* t, SDL_Rect r) : texture(t), rect(r) {}
};

class Graphics {

	// ---------- CONSTRUCTORS ----------

public :
	Graphics();
	Graphics(const char window_name[]);

	// ---------- ACCESSORS ----------

    // ---------- METHODS ----------

public :
	void start();
	void add_wall(const Wall* wall_to_add);
	void add_corner(const corner* corner_to_show);
	void add_rays(const RealAntenna* transmitter);
	void add_text(const char text[], const Vector& pos, const color& col); // pos is here the center of the text label
	void add_tiles(const tileVect& tiles, bool dBm = false);
private :
	void add_line(const Vector& start, const Vector& end, const color& col);
	void add_rect(const Vector& start, int length, int width, const color& col); // Start is here considered as the top of the rectangle with the width on the x axis
	void close();
	void set_colormap_scale(float min, float max);
	const color colormap(float value, Uint8 alpha = 100) const;
	void add_colormap_legend(const char txt1[] = "MAXIMUM", const char txt2[] = "", const char txt3[] = "", const char txt4[] = "MINIMUM");
	const Vector to_pixel(const Vector& v) const;
	int to_pixel(float f) const;
	int to_pixel(int i) const;

	// ---------- ATTRIBUTES ----------

private :
	SDL_Window* window;
	SDL_Surface* surface;
	SDL_Renderer* renderer;
	TTF_Font* font;
	std::vector<colored_rect> rectangles;
	std::vector< colored_line> lines;
	std::vector<txt> texts;
	float min_value;
	float max_value;
	const Vector offset; // displace the origin
};
