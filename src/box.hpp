#ifndef BOX_HPP
#define BOX_HPP
#include <iostream>

#include <SDL2/SDL.h>

#include "texturehandler.hpp"
#include "constants.hpp"
#include "position.hpp"

class Box {
public:
	Box(int x = 0, int y = 0, int w = 0, int h = 0);
	~Box();

	void set_x(int x);
	void set_y(int y);
	void set_width(int w);
	void set_height(int h);

	void update_pair();
	void render(SDL_Renderer* renderer, TextureHandler& textures, int offset_x = 0, int offset_y = 0);
private:
    int x;
	int y;
	int w;
	int h;

	std::array<std::pair<int, SDL_Rect>, 9> pos;
	SDL_Rect src;
};

#endif
