#include "generic.hpp"

bool Generic::collision(SDL_Rect r1, SDL_Rect r2)
{
    return Generic::collision(r1.x, r1.y, r1.w, r1.h, r2.x, r2.y, r2.w, r2.h);
}

void Generic::Render::renderRepeating(SDL_Renderer* renderer, TextureHandler& textures,
                                      int texture, int clipWidth,
                                      int clipHeight, int offsetX, int offsetY,
                                      int width, int height, int gap, int top,
                                      bool verticle, int srcW, int srcH)
{
    srcW = srcW == -1 ? width : srcW;
    srcH = srcH == -1 ? height : srcH;
	
	// Create a grid of tiles as the background
	for (int i = -1; (width + gap) * i < clipWidth + (width + gap); i++) // Horizontal grid
    {
		for (int j = -1; verticle ? (height + gap) * j < clipHeight + (height + gap) : j < 0; j++) // Vertical grid
        {
			SDL_Rect src{0, 0, srcW, srcH};
            
		    SDL_Rect block{(offsetX % (width + gap)) + ((width + gap) * i),
				verticle ? ((offsetY % (height + gap)) + top) + ((height + gap) * j) : top + offsetY,
				width, height};

            SDL_RenderCopy(renderer, textures.getTexture(texture)->texture, &src, &block);
		}
	}
}
