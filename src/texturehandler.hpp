#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include <utility>
#include <string>

#include <SDL2/SDL.h>

#include "texture.hpp"
#include "constants.hpp"

struct TextureHandler
{
    TextureHandler(SDL_Renderer* renderer);
	~TextureHandler();

    SDL_Texture* getTexture(int id);
	void freeTextures();
private:
	std::vector<std::pair<int, Texture*>> textures;
};
