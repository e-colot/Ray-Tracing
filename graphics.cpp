#include "graphics.h"
#include <iostream>
#include <cmath>

#define ABS(x) ((x>=0)? x : -x)

// Constructors

Graphics::Graphics() : Graphics("No name") {}
Graphics::Graphics(const char name[]) : min_value(0.0f), max_value(1.0f), offset((EXERCISE) ? Vector(750, 50) : Vector(200, 25)) {
	window = NULL;
	surface = NULL;
	renderer = NULL;
	font = NULL;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else {
		//Create window
		window = SDL_CreateWindow(name, 2000, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL) {
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else {
			//Create renderer for window
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			if (renderer == NULL) {
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
			}
			else {
				//Initialize renderer color
				SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0xFF);
			}

		}
		if (TTF_Init() == -1) {
			printf("TTF could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		}
		font = TTF_OpenFont("D:\\Documents\\Codage\\C++\\NewProject\\Project1\\fonts\\Bebas-Neue-master\\fonts\\BebasNeue(2018)ByDhamraType\\ttf\\BebasNeue-Regular.ttf", 20);
		if (font == NULL) {
			printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
		}
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	}
}

// Destructor

Graphics::~Graphics() {
	delete surface;
	for (const colored_rect* r : rectangles) {
		delete r;
	}
	for (const colored_line* l : lines) {
		delete l;
	}
	for (const txt* t : texts) {
		delete t;
	}
}

// Methods

void Graphics::start() {
	bool quit = false;

	//Event handler
	SDL_Event e;

	//While application is running
	while (!quit) {
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0) {
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
		}

		SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0xFF);
		SDL_RenderClear(renderer);

		for (int i = static_cast<int>(rectangles.size())-1; i >= 0; i--) {
			SDL_SetRenderDrawColor(renderer, rectangles[i]->c.r, rectangles[i]->c.g, rectangles[i]->c.b, rectangles[i]->c.a);
			SDL_RenderFillRect(renderer, &rectangles[i]->rect);
		}
		for (int i = static_cast<int>(lines.size()) - 1; i >= 0; i--) {
			SDL_SetRenderDrawColor(renderer, lines[i]->c.r, lines[i]->c.g, lines[i]->c.b, lines[i]->c.a);
			SDL_RenderDrawLine(renderer, lines[i]->x1, lines[i]->y1, lines[i]->x2, lines[i]->y2);
		}
		for (int i = static_cast<int>(texts.size()) - 1; i >= 0; i--) {
			SDL_RenderCopy(renderer, texts[i]->texture, NULL, &(texts[i]->rect));
		}

		//Update screen
		SDL_RenderPresent(renderer);
		SDL_Delay(100);
	}
	//Free resources and close SDL
	close();
}
void Graphics::add_wall(const Wall* w) {
	for (int i = 0; i < static_cast<int>(w->get_intervals().size()); i += 2) {
		if (i + 1 != static_cast<int>(w->get_intervals().size())) {
			Vector start;
			// for each part of the wall
			switch (ABS(static_cast<int>(100*w->get_dir().get_x()))) {
				// *100 because the dir vector is normed and with a diagonal vector, it would be rounded to 0 or 1 making it not detectable
			case (0) :
				switch (static_cast<int>(w->get_dir().get_y())) {
				case (1) :
					start = to_pixel(w->get_pos() + w->get_intervals()[i] * w->get_dir());
					break;
				case (-1) :
					start = to_pixel(w->get_pos() + w->get_intervals()[i+1] * w->get_dir());
					break;
				}
				add_rect(start, to_pixel(w->get_intervals()[i + 1] - w->get_intervals()[i]), to_pixel(w->get_material(i)->get_thickness()), w->get_material(i)->get_color());
				break;
			case (100) :
				start = to_pixel(w->get_pos() + (w->get_intervals()[i] + (0.5f*(w->get_intervals()[i + 1] - w->get_intervals()[i]))) * w->get_dir() + w->get_material(i)->get_thickness() * Vector(0.0f, -0.5f));
				add_rect(start, to_pixel(w->get_material(i)->get_thickness()), to_pixel(w->get_intervals()[i + 1] - w->get_intervals()[i]), w->get_material(i)->get_color());
				break;
			default:
				// if the wall is diagonal, it is displayed as a line because a rotated rectangle is not easily drawable with SDL
				start = to_pixel(w->get_pos() + w->get_intervals()[i] * w->get_dir());
				Vector end = to_pixel(w->get_pos() + w->get_intervals()[i+1] * w->get_dir());
				for (int j = to_pixel(-w->get_material(i)->get_thickness()); j <= to_pixel(w->get_material(i)->get_thickness()); j++) {
					add_line(start + Vector(0, j), end + Vector(j, 0), w->get_material(i)->get_color());
				}
				break;
			}
		}
	}
}
void Graphics::add_corner(const corner* c) {
	add_rect(to_pixel(c->pos + c->mat->get_thickness() * Vector(0.0f, -0.5f)), to_pixel(c->mat->get_thickness()), to_pixel(c->mat->get_thickness()), c->mat->get_color());
}
void Graphics::add_rays(const RealAntenna* tx) {
	set_colormap_scale(tx->get_min_attenuation(), tx->get_max_attenuation());
	for (const Ray* r : tx->get_rays()) {
		color c;
		c = colormap(r->get_attenuation());
		for (const Path* p : r->get_path()) {
			add_line(to_pixel(p->get_start()), to_pixel(p->get_end()), c);
		}
	}
	add_colormap_legend();
}
void Graphics::add_text(const char text[], const Vector& p, const color& c) {
	if (*text != '\0') {
		// if text is not empty
		SDL_Color txt_color = { c.r, c.g, c.b, c.a };
		SDL_Surface* txt_surf = TTF_RenderText_Solid(font, text, txt_color);
		if (txt_surf == NULL) {
			printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
		}
		else {
			SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, txt_surf);
			if (textTexture == NULL) {
				printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
			}
			else {
				SDL_Rect textRect = SDL_Rect({ static_cast<int>(p.get_x() - txt_surf->w / 2.0f), static_cast<int>(p.get_y() - txt_surf->h / 2.0f), txt_surf->w, txt_surf->h });
				texts.push_back(new txt(textTexture, textRect));
			}
		}
	}
}
void Graphics::add_tiles(const tileVect& tiles, bool dBm) {
	for (Tile* t : tiles) {
		color c;
		if (dBm) {
			set_colormap_scale(62.47245f, 106.0206f);
			if (t->get_rate(0) == 0) {
				c = colormap(62.47245f, static_cast<Uint8>(75));
				// set to the minimum value of the colormap
			}
			else {
				c = colormap(10 * log10(t->get_rate(0)), static_cast<Uint8>(75));
			}
		}
		else {
			set_colormap_scale(0, 40e9f);
			c = colormap(t->get_rate(0), static_cast<Uint8>(75));
		}
		add_rect(to_pixel(t->get_pos() + Vector(0.0f, -TILE_SIZE / 2.0f)), to_pixel(TILE_SIZE), to_pixel(TILE_SIZE), c);
	}
	if (dBm) {
		add_colormap_legend("40 GB/s", "1.4 GB/s", "40 MB/s", "0  GB/s");
	}
	else {
		add_colormap_legend("40 GB/s", "30 GB/s", "10 GB/s", "0  GB/s");
	}
}
void Graphics::set_colormap_scale(double min, double max) {
	// small changes to make sure we stay in the interval even with the lost of precision from double to float
	min_value = min-0.001;  
	max_value = max+0.001;
}
void Graphics::add_colormap_legend(const char txt1[], const char txt2[], const char txt3[], const char txt4[]) {
	for (int i = 255; i >= 0; i--)
	{
		color c = colormap(min_value + ((255.0f - i)/255.0f) * (max_value - min_value), static_cast<Uint8>(255));
		add_line(Vector(SCREEN_WIDTH - 80, 100 + 2 * i), Vector(SCREEN_WIDTH - 40, 100 + 2 * i), c);
		add_line(Vector(SCREEN_WIDTH - 80, 101 + 2 * i), Vector(SCREEN_WIDTH - 40, 101 + 2 * i), c);
	}
	add_text(txt4, Vector(SCREEN_WIDTH - 120, 610), color({ 255, 255, 255, 255 }));
	add_text(txt3, Vector(SCREEN_WIDTH - 120, 440), color({ 255, 255, 255, 255 }));
	add_text(txt2, Vector(SCREEN_WIDTH - 120, 270), color({ 255, 255, 255, 255 }));
	add_text(txt1, Vector(SCREEN_WIDTH - 120, 100), color({ 255, 255, 255, 255 }));
}
void Graphics::add_line(const Vector& start, const Vector& end, const color& col) {
	colored_line* line = new colored_line(start, end, col);
	lines.push_back(line);
}
void Graphics::add_rect(const Vector& start, int length, int width, const color& col) {
	colored_rect* rect = new colored_rect(start, length, width, col);
	rectangles.push_back(rect);
}
void Graphics::close() {
	// Destroy window	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	window = NULL;
	renderer = NULL;

	// Quit SDL subsystems
	SDL_Quit();
}
const color Graphics::colormap(double value, Uint8 alpha) const {
	// color first determined in HSV for a better maping
	double fraction = (value - min_value) / (max_value - min_value);
	float h = static_cast<float>(160.0f * (1.0f - fraction)) / 255.0f; // Teinte Hue
	float s = 1.0f; // Saturation
	float v = 1.0f; // Value

	int i = static_cast<int>(floor(h * 6));
	float f = h * 6 - i;
	float p = v * (1 - s);
	float q = v * (1 - f * s);
	float t = v * (1 - (1 - f) * s);

	float r = 1.0f, g = 1.0f, b = 1.0f;
	switch (i % 6) {
	case 0: r = v, g = t, b = p; break;
	case 1: r = q, g = v, b = p; break;
	case 2: r = p, g = v, b = t; break;
	case 3: r = p, g = q, b = v; break;
	case 4: r = t, g = p, b = v; break;
	case 5: r = v, g = p, b = q; break;
	}

	return color{ static_cast<Uint8>(r * 255), static_cast<Uint8>(g * 255), static_cast<Uint8>(b * 255), alpha };
}
const Vector Graphics::to_pixel(const Vector& v) const {
	return offset + (SCALE * v);
}
int Graphics::to_pixel(float f) const {
	return static_cast<int>(SCALE * f);
}
int Graphics::to_pixel(int i) const {
	return SCALE * i;
}
