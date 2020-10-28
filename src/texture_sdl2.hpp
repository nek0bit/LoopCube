// Only include if enabled
#ifdef GRAPHIC_BACKEND_SDL2
#include "backendincludes.hpp"

#ifndef TEXTURE_SDL2_HPP
#define TEXTURE_SDL2_HPP
#include <string>

#include "texture.hpp"

class Texture_SDL2: public Texture<SDL_Texture*> {
public:
	Texture_SDL2(SDL_Renderer* renderer, std::string filename);
	~Texture_SDL2();

    SDL_Texture* get_texture();
	void free_texture() override;
private:
	SDL_Texture* texture;
};

#endif // TEXTURE_SDL2_HPP

#endif // GRAPHIC_BACKEND_SDL2
