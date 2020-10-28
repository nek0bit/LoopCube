// TODO Delete me if not needed anymore for reference

#include "texture.hpp"

Texture::Texture(SDL_Renderer* renderer, std::string filename) {
	SDL_Surface* surface = IMG_Load(filename.c_str());
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	// Enable opacity
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	
	this->texture = texture;
}

Texture::~Texture() {
}

SDL_Texture* Texture::get_texture() {
	return texture;
}

void Texture::free_texture() {
	SDL_DestroyTexture(texture);
}
