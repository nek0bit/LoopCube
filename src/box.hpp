#pragma once
#include <iostream>
#include <array>
#include <utility>

#include "graphics.hpp"
#include "texturehandler.hpp"
#include "constants.hpp"

struct Box
{
	Box(int x = 0, int y = 0, int w = 0, int h = 0);
    
    int x;
	int y;
	int w;
	int h;

	void updatePair();
	void render(const Graphics& graphics, TextureHandler& textures, int offsetX = 0, int offsetY = 0) const noexcept;
private:
	std::array<std::pair<int, SDL_Rect>, 9> pos;
	SDL_Rect src;
};
