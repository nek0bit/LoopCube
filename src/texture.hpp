#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include <SDL2/SDL_image.h>
#include <string>

class Texture {

public:
	Texture(SDL_Renderer* renderer, std::string filename);
	~Texture();

	SDL_Texture* get_texture();
	void free_texture();

private:
	SDL_Texture* texture;
};


#endif // TEXTURE_HPP
