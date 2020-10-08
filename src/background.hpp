#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP
#include <iostream>

#include <SDL2/SDL.h>

#include "constants.hpp"
#include "texturehandler.hpp"
#include "camera.hpp"

class Background {
public:
	Background();
	~Background();
	
	void update(Camera& camera);
	void render(SDL_Renderer* renderer, TextureHandler& textures);
private:
	void render_repeating(SDL_Renderer* renderer, TextureHandler& textures, int texture, int offset_x, int offset_y, int width,
						  int height, int gap, int top);
	int win_width;

	SDL_Rect bg_shine_src, bg_shine_dest;
	int bg_cloud_offset_x;
	int bg_cloud_offset_y;
	int bg_hills_offset_x;
	int bg_hills_offset_y;
	
	
	int bg_shine_w;
	int bg_shine_h;
	int bg_cloud_w;
	int bg_cloud_h;
	int bg_hills_w;
	int bg_hills_h;
};

#endif
