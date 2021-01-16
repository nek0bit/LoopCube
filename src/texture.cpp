#include "texture.hpp"

Texture::Texture(SDL_Renderer* renderer, std::string filename)
{
    SDL_Surface* surface = IMG_Load(filename.c_str());
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	// Enable opacity for the texture
	// Not all textures use opacity but I don't think this harms a lot
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	
	this->texture = texture;
}

Texture::~Texture()
{
    freeTexture();
}

SDL_Texture*& Texture::getTexture()
{
    return texture;
}

void Texture::freeTexture()
{
    SDL_DestroyTexture(texture);
}
