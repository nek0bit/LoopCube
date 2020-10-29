#include "texture_sdl2.hpp"

Texture_SDL2::Texture_SDL2(SDL_Renderer* renderer, std::string filename) : Texture<SDL_Texture*>{} {
	SDL_Surface* surface = IMG_Load(filename.c_str());
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	// Enable opacity for the texture
	// Honestly not all textures use opacity but I don't think this harms a lot
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	
	this->texture = texture;
}

Texture_SDL2::~Texture_SDL2() {
	free_texture();
}

SDL_Texture* Texture_SDL2::get_texture() {
	return texture;
}

void Texture_SDL2::free_texture() {
	SDL_DestroyTexture(texture);
}
