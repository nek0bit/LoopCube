#include "box.hpp"

Box::Box(int x, int y, int w, int h) : x{x}, y{y}, w{w}, h{h},
									   src{0, 0, 40, 40} {
	update_pair();
}

Box::~Box() {}

void Box::update_pair() {
	pos = std::array<std::pair<int, SDL_Rect>, 9>{
		std::make_pair(22, SDL_Rect{x, y, src.w, src.h}), // Top left
		std::make_pair(23, SDL_Rect{x+src.w, y, w-(src.w*2), src.h}), // Top middle
		std::make_pair(24, SDL_Rect{x+w-src.w, y, src.w, src.h}), // Top right
		std::make_pair(25, SDL_Rect{x, y+src.h, src.w, h-(src.h*2)}), // Middle left
		std::make_pair(26, SDL_Rect{x+src.w, y+src.h, w-(src.w*2), h-(src.h*2)}), // Center
		std::make_pair(27, SDL_Rect{x+w-src.w, y+src.h, src.w, h-(src.h*2)}), // Middle right
		std::make_pair(28, SDL_Rect{x, y+h-src.h, src.w, src.h}), // Bottom left
		std::make_pair(29, SDL_Rect{x+src.w, y+h-src.h, w-(src.w*2), src.h}), // Bottom middle
		std::make_pair(30, SDL_Rect{x+w-src.w, y+h-src.h, src.w, src.h}) // Bottom right
	};
}

void Box::set_x(int x) {
	this->x = x;
}
void Box::set_y(int y) {
	this->y = y;
}
void Box::set_width(int w) {
	this->w = w;
}
void Box::set_height(int h) {
	this->h = h;
}

void Box::render(SDL_Renderer* renderer, TextureHandler& textures, int offset_x, int offset_y) {
	for (auto& p: pos) {
		SDL_Rect mod = p.second;
		mod.x += offset_x;
		mod.y += offset_y;
        SDL_RenderCopy(renderer, textures.get_texture(p.first), &src, &mod);
	}
}
