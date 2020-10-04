#include "checkbox.hpp"

Checkbox::Checkbox(int id, std::string text, int x, int y, int size)
	: id{id}, text{text}, x{x}, y{y}, size{size}, checked{false}, text_render{nullptr} {
}

bool Checkbox::toggle() {
	checked = !checked;
	return checked;
}

void Checkbox::get_checked() {
	return checked;
}

void Checkbox::check() {
	checked = true;
}

void Checkbox::uncheck() {
	checked = false;
}

void Checkbox::update(EventHandler& events) {
	std::array<int, 2> pos = events.get_mouse_pos();
	if (events.get_mouse_clicked() &&
		AABB(x, y, width, height, pos[0], pos[1], 1, 1)) {
		toggle();
	}
}

void Checkbox::render(SDL_Renderer* renderer, int x_offset, int y_offset) {
	SDL_Rect box;
	box.x = x;
	box.y = y;
	box.w = width;
	box.h = height;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
	SDL_RenderFillRect(renderer, &box);

	if (text_render == nullptr) {
		SDL_Color color;
		color.r = 255; color.g = 255; color.b = 255; color.a = 255;
		text_render = new Text(renderer, text, color, constants::button_font); 
	} else {
		text_render->draw(x+width+10, y+5
	}
}
