#include "background.hpp"

Background::Background()
	: bg_shine_src{}, bg_shine_dest{} {
	// Set size for objects
	bg_shine_w = 10; //px
	bg_shine_h = 150;
}

Background::~Background() {}

void Background::update(Camera& camera) {
	// Update bg_shine
	bg_shine_src.x = 0;
	bg_shine_src.y = 0;
	bg_shine_src.w = bg_shine_w;
	bg_shine_src.h = bg_shine_h;

	bg_shine_dest.x = 0;
	bg_shine_dest.y = 0;
	bg_shine_dest.w = camera.get_width();
	bg_shine_dest.h = camera.get_height();
	
}

void Background::render(SDL_Renderer* renderer, TextureHandler& textures) {
	// Render bg_shine
	SDL_RenderCopy(renderer, textures.get_texture(13), &bg_shine_src, &bg_shine_dest);
}
