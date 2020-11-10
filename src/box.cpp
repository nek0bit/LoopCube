#include "box.hpp"

Box::Box(int x, int y, int w, int h) : x{x}, y{y}, w{w}, h{h},
									   src{0, 0, 40, 40} {
	update_pair();
}

Box::~Box() {}

void Box::update_pair() {
	pos = std::array<std::pair<int, Rect>, 9>{
		std::make_pair(22, Rect{x, y, src.w, src.h}),
		std::make_pair(23, Rect{x+src.w, y, w-src.w, src.h}),
		std::make_pair(24, Rect{x+w, y, src.w, src.h}),
		std::make_pair(25, Rect{x, y+src.h, src.w, h-src.h}),
		std::make_pair(26, Rect{x+src.w, y+src.h, w-src.w, h-src.h}),
		std::make_pair(27, Rect{x+w, y+src.h, src.w, h-src.h}),
		std::make_pair(28, Rect{x, y+h, src.w, src.h}),
		std::make_pair(29, Rect{x+src.w, y+h, w-src.w, src.h}),
		std::make_pair(30, Rect{x+w, y+h, src.w, src.h})
	};
}

void Box::set_x(int x) {
	this->x = x;
	update_pair();
}
void Box::set_y(int y) {
	this->y = y;
	update_pair();
}
void Box::set_width(int w) {
	this->w = w;
	update_pair();
}
void Box::set_height(int h) {
	this->h = h;
	update_pair();
}

void Box::render(GraphicsWrapper* renderer) {
	for (auto& p: pos) {
		renderer->render(src, p.second, p.first);
	}
}
