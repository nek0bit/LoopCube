#ifndef TEXTUREHANDLER_HPP
#define TEXTUREHANDLER_HPP
#include <iostream>
#include <algorithm>
#include <vector>
#include <utility>
#include <string>

#include "backendincludes.hpp"

#include "texture.hpp"
#include "constants.hpp"

class TextureHandler {
public:
	// Pass a function (lambda preferable) that returns the created texture
	TextureHandler(Texture* (*create_texture)(std::string&));
	~TextureHandler();

    Texture* get_texture(int id);
	void free_textures();
private:
	std::vector<std::pair<int, Texture*>> textures;
};


#endif // TEXTUREHANDLER_HPP
