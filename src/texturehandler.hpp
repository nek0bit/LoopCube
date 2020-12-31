#ifndef TEXTUREHANDLER_HPP
#define TEXTUREHANDLER_HPP
#include <iostream>
#include <algorithm>
#include <vector>
#include <utility>
#include <string>

#include "texture.hpp"
#include "constants.hpp"

class TextureHandler {
public:
    TextureHandler();
	~TextureHandler();

    SDL_Texture*& get_texture(int id);
	void free_textures();
private:
	std::vector<std::pair<int, T>> textures;
};


#endif // TEXTUREHANDLER_HPP
