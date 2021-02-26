#include "box.hpp"

Box::Box(int x, int y, int w, int h)
    : x{x},
      y{y},
      w{w},
      h{h},
      src{0, 0, 40, 40}
{
    updatePair();
}

void Box::updatePair()
{
	pos = std::array<std::pair<int, SDL_Rect>, 9> {
		std::make_pair(TEXTURE_BOX1, SDL_Rect{x, y, src.w, src.h}), // Top left
		std::make_pair(TEXTURE_BOX2, SDL_Rect{x+src.w, y, w-(src.w*2), src.h}), // Top middle
		std::make_pair(TEXTURE_BOX3, SDL_Rect{x+w-src.w, y, src.w, src.h}), // Top right
		std::make_pair(TEXTURE_BOX4, SDL_Rect{x, y+src.h, src.w, h-(src.h*2)}), // Middle left
		std::make_pair(TEXTURE_BOX5, SDL_Rect{x+src.w, y+src.h, w-(src.w*2), h-(src.h*2)}), // Center
		std::make_pair(TEXTURE_BOX6, SDL_Rect{x+w-src.w, y+src.h, src.w, h-(src.h*2)}), // Middle right
		std::make_pair(TEXTURE_BOX7, SDL_Rect{x, y+h-src.h, src.w, src.h}), // Bottom left
		std::make_pair(TEXTURE_BOX8, SDL_Rect{x+src.w, y+h-src.h, w-(src.w*2), src.h}), // Bottom middle
		std::make_pair(TEXTURE_BOX9, SDL_Rect{x+w-src.w, y+h-src.h, src.w, src.h}) // Bottom right
	};
}

void Box::render(const Graphics& renderer, TextureHandler& textures, int offsetX, int offsetY)
{
	for (auto& p: pos)
    {
		SDL_Rect mod = p.second;
		mod.x += offsetX;
		mod.y += offsetY;
        //SDL_RenderCopy(renderer, textures.getTexture(p.first)->texture, &src, &mod);
	}
}
