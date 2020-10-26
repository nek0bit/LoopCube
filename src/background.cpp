#include "background.hpp"

Background::Background()
	: win_width{0}, show_cave_background{false}, bg_shine_src{}, bg_shine_dest{}, bg_cloud_offset_x{0}, bg_cloud_offset_y{0}, bg_cave_block_x{0}, bg_cave_block_y{0} {
	// Set size for objects
	bg_shine_w = 10; //px
	bg_shine_h = 150;

	bg_cloud_w = 450;
	bg_cloud_h = 100;

	bg_hills_w = 450*2;
	bg_hills_h = 155*2;

	bg_cave_block_w = 60;
	bg_cave_block_h = 60;
}

Background::~Background() {}

void Background::update(Camera& camera) {
	const int cave_background_offset = -7269;
	
	win_width = camera.get_width();
	win_height = camera.get_height();
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

	// Enable the cave background is camera is low enough
	show_cave_background = camera.get_y() < cave_background_offset;
	bg_cave_block_x = camera.get_x()/10;
	bg_cave_block_y = camera.get_y()/10;
}

void Background::render(SDL_Renderer* renderer, TextureHandler& textures) {
	static double cavebg_opacity = 1;
	
	const double transition = 255;

	if (show_cave_background) {
		cavebg_opacity += ((transition-cavebg_opacity)/4);
	} else {
		cavebg_opacity += ((0-cavebg_opacity)/4);
	}
	if (cavebg_opacity > transition-1) cavebg_opacity = 255;
	
	int offset = 200;
	
	// Render bg_shine
	SDL_RenderCopy(renderer, textures.get_texture(13), &bg_shine_src, &bg_shine_dest);

	// Render clouds
	render_repeating(renderer, textures, 14, bg_cloud_offset_x, bg_cloud_offset_y,
					 bg_cloud_w, bg_cloud_h, 60, 40 + offset);

	// Render hills
	render_repeating(renderer, textures, 16, bg_hills_offset_x, bg_hills_offset_y,
					 bg_hills_w, bg_hills_h, 0, 300 + offset);

	if (cavebg_opacity > 2) {
		// Set opacity
		int texture = 17;
		SDL_Texture* mod_texture = textures.get_texture(texture);
		SDL_SetTextureAlphaMod(mod_texture, static_cast<int>(cavebg_opacity));
		
		render_repeating(renderer, textures, texture, bg_cave_block_x, bg_cave_block_y,
						 bg_cave_block_w, bg_cave_block_h, 0, 0, true);

		// Reset opacity
		SDL_SetTextureAlphaMod(mod_texture, 255);
	}
}

void Background::render_repeating(SDL_Renderer* renderer, TextureHandler& textures, int texture, int offset_x, int offset_y, int width,
										 int height, int gap, int top, bool verticle) {
	const int MAX_X = win_width;
	const int MAX_Y = win_height;
	// Create a grid of tiles as the background
	for (int i = -1; (width+gap)*i < MAX_X+(width+gap); i++) {
		for (int j = -1; verticle ? (height+gap)*j < MAX_Y+(height+gap) : j < 0; j++) {
			SDL_Rect block{(offset_x % (width+gap))+((width+gap)*i),
				verticle ? ((offset_y % (height+gap)) + top)+((height+gap)*j) : top+offset_y,
				width, height};
			SDL_Rect src{0, 0, width, height};

			// Draw the tile
			SDL_RenderCopy(renderer, textures.get_texture(texture), &src, &block);
		}
	}
}
