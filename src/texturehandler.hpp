#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include <utility>
#include <string>

#include <SDL2/SDL.h>

#include "texture.hpp"
#include "constants.hpp"

struct TextureHandler
{
    TextureHandler();
	~TextureHandler();

    void init();

    inline Texture const* getTexture(const size_t id) const noexcept {
        return &textures[id];
    }
private:
	std::vector<Texture> textures;
};
