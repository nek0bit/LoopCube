#ifndef TEXTUREHANDLER_HPP
#define TEXTUREHANDLER_HPP
#include <algorithm>
#include <vector>
#include <utility>
#include <SDL2/SDL.h>

#include "texture.hpp"
#include "constants.hpp"

class TextureHandler {

public:
	TextureHandler() = default;
	TextureHandler(SDL_Renderer* renderer);
	~TextureHandler();

	SDL_Texture* get_texture(int id);
	void free_textures();
private:

	SDL_Renderer* renderer;
	std::vector<std::pair<int, Texture>> textures;
};


#endif // TEXTUREHANDLER_HPP
