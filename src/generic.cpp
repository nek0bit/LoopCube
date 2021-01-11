#include "generic.hpp"

bool Generic::collision(SDL_Rect r1, SDL_Rect r2)
{
    return Generic::collision(r1.x, r1.y, r1.w, r1.h, r2.x, r2.y, r2.w, r2.h);
}

void Generic::Render::renderRepeating(SDL_Renderer* renderer, TextureHandler& textures, int texture, int offsetX, int offsetY, int width,
                                      int height, int gap, int top, bool verticle = false, int srcW = -1, int srcH = -1)
{
    if (src_w == -1) src_w = width;
	if (src_h == -1) src_h = height;
	
	const int MAX_X = win_width;
	const int MAX_Y = win_height;
	// Create a grid of tiles as the background
	for (int i = -1; (width+gap)*i < MAX_X+(width+gap); i++) {
		for (int j = -1; verticle ? (height+gap)*j < MAX_Y+(height+gap) : j < 0; j++) {
			SDL_Rect src{0, 0, src_w, src_h};
            
		    SDL_Rect block{(offset_x % (width+gap))+((width+gap)*i),
				verticle ? ((offset_y % (height+gap)) + top)+((height+gap)*j) : top+offset_y,
				width, height};

			// Draw the tile
            SDL_RenderCopy(renderer, textures.get_texture(texture), &src, &block);
		}
	}
}
