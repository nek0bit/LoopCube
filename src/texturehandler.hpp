#pragma once
#include <iostream>
#include <algorithm>
#include <memory>
#include <vector>
#include <utility>
#include <string>
#include <unordered_map>

#include <SDL2/SDL.h>

#include "texture.hpp"
#include "constants.hpp"

struct TextureHandler
{
    TextureHandler();
	~TextureHandler();

    Texture* getTexture(size_t id);    
private:
	std::vector<std::pair<int, std::shared_ptr<Texture>>> textures;
};
