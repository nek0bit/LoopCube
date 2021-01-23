#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include <utility>
#include <string>
#include <unordered_map>

#include <SDL2/SDL.h>

#include "texture.hpp"
#include "constants.hpp"

struct TextureHandler
{
    TextureHandler(SDL_Renderer* renderer);
	~TextureHandler();

    Texture* getTexture(size_t id);
    
	void freeTextures();
private:
	std::vector<std::pair<int, Texture*>> textures;
};
