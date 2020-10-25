#include "background.hpp"

Background::Background()
	: win_width{0}, bg_shine_src{}, bg_shine_dest{}, bg_cloud_offset_x{0}, bg_cloud_offset_y{0} {
	// Set size for objects
	bg_shine_w = 10; //px
	bg_shine_h = 150;

	bg_cloud_w = 450;
	bg_cloud_h = 100;

	bg_hills_w = 450*2;
	bg_hills_h = 155*2;
}

Background::~Background() {}

void Background::update(Camera& camera) {
	win_width = camera.get_width();
	// Update bg_shine
	bg_shine_src.x = 0;
	bg_shine_src.y = 0;
	bg_shine_src.w = bg_shine_w;
	bg_shine_src.h = bg_shine_h;

	bg_shine_dest.x = 0;
	bg_shine_dest.y = 0;
	bg_shine_dest.w = camera.get_width();
	bg_shine_dest.h = camera.get_height();

	bg_cloud_offset_x = camera.get_x()/10;
	bg_cloud_offset_y = camera.get_y()/30;

	bg_hills_offset_x = camera.get_x()/15;
	bg_hills_offset_y = camera.get_y()/30;
}

void Background::render(SDL_Renderer* renderer, TextureHandler& textures) {
	int offset = 200;
	
	// Render bg_shine
	SDL_RenderCopy(renderer, textures.get_texture(13), &bg_shine_src, &bg_shine_dest);

	// Render clouds
	render_repeating(renderer, textures, 14, bg_cloud_offset_x, bg_cloud_offset_y, bg_cloud_w, bg_cloud_h, 60, 50 + offset);

	// Render hills
	render_repeating(renderer, textures, 16, bg_hills_offset_x, bg_hills_offset_y, bg_hills_w, bg_hills_h, 0, 300 + offset);
}

void Background::render_repeating(SDL_Renderer* renderer, TextureHandler& textures, int texture, int offset_x, int offset_y, int width,
								  int height, int gap, int top) {
	int i = -1;
	do {
		SDL_Rect dest{(offset_x % (width+gap))+((width+gap)*i), top+offset_y, width, height};
		SDL_Rect src{0, 0, width, height};
		SDL_RenderCopy(renderer, textures.get_texture(texture), &src, &dest); 
		i++;
	} while ((width+gap)*i < win_width+(width+gap));
}
