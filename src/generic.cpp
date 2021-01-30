#include "generic.hpp"

bool Generic::collision(SDL_Rect r1, SDL_Rect r2)
{
    return Generic::collision(r1.x, r1.y, r1.w, r1.h, r2.x, r2.y, r2.w, r2.h);
}

GridCollision_t Generic::gridCollision(unsigned int width, unsigned int height, Vec2 box, const Size& size)
{
    uint8_t add = 0; // 1 = X, 2 = Y, 3 = XY
    // negative fix
    if (box.x < 0) {
        box.x -= width + 1;
        add = add | 1;
    }   
    if (box.y < 0) {
        box.y -= height + 1;
        add = add | 2;
    }
    
    // Constrain coordinates
    box.x = std::fmod(box.x, width);
    box.y = std::fmod(box.y, height);

    return {((size.w + box.x) / width) + (add & 1 == 1 ? 2 : 1),
        ((size.h + box.y) / height) + (add & 2 == 2 ? 2 : 1)};
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
