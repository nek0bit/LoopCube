#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP
#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>

#include <SDL2/SDL.h>

#include "constants.hpp"
#include "texturehandler.hpp"
#include "camera.hpp"
#include "time.hpp"
#include "transition.hpp"

class Background {
public:
	Background();
	~Background();
	
	void update(Camera& camera, Time& time);
	void render(SDL_Renderer* renderer, TextureHandler* textures);
	void render_light(SDL_Renderer* renderer, TextureHandler* textures, Camera& camera);
private:
	void render_repeating(SDL_Renderer* renderer, TextureHandler* textures,  int texture, int offset_x, int offset_y, int width,
						  int height, int gap, int top, bool verticle = false, int src_w = -1, int src_h = -1);
	int win_width;
	int win_height;

	bool show_cave_background;

	Rect bg_shine_src, bg_shine_dest;
	int bg_cloud_offset_x;
	int bg_cloud_offset_y;
	int bg_hills_offset_x;
	int bg_hills_offset_y;
	int bg_cave_block_x;
	int bg_cave_block_y;

	Rect bg_light_src, bg_light_dest;
	Rect bg_moon_src, bg_moon_dest;
	
	int bg_shine_w;
	int bg_shine_h;
	int bg_cloud_w;
	int bg_cloud_h;
	int bg_hills_w;
	int bg_hills_h;
	int bg_cave_block_w;
	int bg_cave_block_h;
	int bg_light_w;
	int bg_light_h;

	// Sky
	
	Transition dark;
	int darkness;

	int r;
	int g;
	int b;
};

#endif
