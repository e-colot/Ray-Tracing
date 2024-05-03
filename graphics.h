#pragma once
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-default"
#include <SDL.h>
#include <SDL_ttf.h>
#pragma GCC diagnostic pop
#include <stdio.h>
#include <vector>
#include "tile.h"

using tileVect = std::vector<Tile*>;

struct colored_rect {
    const SDL_Rect rect; // SDL rectangle for drawing
    const color c; // Color of the rectangle
    colored_rect() : rect{ 0, 0, 0, 0 }, c() {} // Default constructor
    colored_rect(const Vector& start, int length, int width, const color col) : // Constructor with parameters
        rect({ static_cast<int>(start.get_x() - width / 2.0f), static_cast<int>(start.get_y()), width, length }), c(col) {}
};

struct colored_line {
    const int x1; // X-coordinate of the start of the line
    const int y1; // Y-coordinate of the start of the line
    const int x2; // X-coordinate of the end of the line
    const int y2; // Y-coordinate of the end of the line
    const color c; // Color of the line
    colored_line() : x1(0), y1(0), x2(0), y2(0), c() {} // Default constructor
    colored_line(const Vector& start, const Vector& end, color col) : // Constructor with parameters
        x1(static_cast<int>(start.get_x())), y1(static_cast<int>(start.get_y())), x2(static_cast<int>(end.get_x())),
        y2(static_cast<int>(end.get_y())), c(col) {}
};

struct corner {
    const Vector pos; // Position of the corner
    const Material* mat; // Material of the corner
    corner() : pos(Vector()), mat(new Material()) {} // Default constructor
    corner(Vector position, const Material* material) : pos(position), mat(material) {} // Constructor with parameters
};

struct txt {
    SDL_Texture* texture; // SDL texture for text rendering
    const SDL_Rect rect; // SDL rectangle for text positioning
    txt(SDL_Texture* t, SDL_Rect r) : texture(t), rect(r) {} // Constructor with parameters
};

class Graphics {
    // Attributes
private:
    SDL_Window* window; // SDL window for rendering
    SDL_Surface* surface; // SDL surface for drawing
    SDL_Renderer* renderer; // SDL renderer for drawing
    TTF_Font* font; // SDL font for text rendering
    std::vector<const colored_rect*> rectangles; // Rectangles to draw
    std::vector<const colored_line*> lines; // Lines to draw
    std::vector<const txt*> texts; // Texts to draw
    double min_value; // Minimum value for colormap scaling
    double max_value; // Maximum value for colormap scaling
    const Vector offset; // Offset for the origin
    float tile_size; // Size of a tile

    // Constructors
public:
    Graphics(); // Default constructor
    Graphics(const char window_name[]); // Constructor with window name

    // Destructor
public:
    ~Graphics(); // Destructor

    // Mutator
public:
    void set_tile_size(float size);

    // Methods
public:
    void start(); // Starts the graphics rendering
    void add_wall(const Wall* wall_to_add); // Adds a wall to the window
    void add_corner(const corner* corner_to_show); // Adds a corner to the window
    void add_rays(const RealAntenna* transmitter, bool logarithmic); // Adds rays to the window
    void add_text(const char text[], const Vector& pos, const color& col); // Adds text to the window
    void add_tiles(const tileVect& tiles, bool dBm = false); // Adds tiles to the window

private:
    void add_line(const Vector& start, const Vector& end, const color& col); // Adds a line to the window
    void add_rect(const Vector& start, int length, int width, const color& col); // Adds a rectangle to the window
    void close(); // Closes the window
    void set_colormap_scale(double min, double max); // Sets the colormap scale
    const color colormap(double value, Uint8 alpha = 100) const; // Returns a color based on the value
    void add_colormap_legend(const char txt1[] = "MAXIMUM", const char txt2[] = "", const char txt3[] = "", const char txt4[] = "MINIMUM"); // Adds a colormap legend
    const Vector to_pixel(const Vector& v) const; // Converts a Vector to pixel coordinates
    int to_pixel(float f) const; // Converts a float to pixel length
    int to_pixel(int i) const; // Converts an integer to pixel length
};
