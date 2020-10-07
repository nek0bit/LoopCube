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
	SDL_Rect bg_shine_src, bg_shine_dest;

	int bg_shine_w;
	int bg_shine_h;
};

#endif
