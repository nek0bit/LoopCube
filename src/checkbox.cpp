#include "checkbox.hpp"

Checkbox::Checkbox(int id, std::string text, int x, int y, int size, bool checked)
	: changed{false}, id{id}, text{text}, x{x}, y{y}, size{size}, checked{checked}, text_render{nullptr} {
}

Checkbox::~Checkbox() {}

void Checkbox::on_change(void (*function)(int, int)) {
	if (changed) {
		(*function)(id, checked ? 1 : 0);
	}
}

int Checkbox::get_id() {
	return id;
}

void Checkbox::get_value(bool& here) {
	here = checked;
}

bool Checkbox::is_changed() {
	return changed;
}

bool Checkbox::toggle() {
	checked = !checked;
	return checked;
}

bool Checkbox::get_checked() {
	return checked;
}

void Checkbox::check() {
	checked = true;
}

void Checkbox::uncheck() {
	checked = false;
}

void Checkbox::update(EventHandler& events) {
	// Clear changed
	changed = false;

	std::array<int, 2> pos = events.get_mouse_pos();
	if (events.get_mouse_clicked() &&
		AABB(x, y, size, size, pos[0], pos[1], 1, 1)) {
		toggle();
		changed = true;
	}

	dest.x = x;
	dest.y = y;
	dest.w = size;
	dest.h = size;
}

void Checkbox::render(SDL_Renderer* renderer) {
	// Draw box behind check

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
	SDL_RenderFillRect(renderer, &dest);

	// Draw check
	if (checked) {
		SDL_Rect check;
		check.x = dest.x+10;
		check.y = dest.y+10;
		check.w = dest.w-20;
		check.h = dest.h-20;

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(renderer, &check);
	}

	

	if (text_render == nullptr) {
		SDL_Color color;
		color.r = 255; color.g = 255; color.b = 255; color.a = 255;
		text_render = new Text(renderer, text, color, constants::option_font); 
	} else {
		text_render->draw((dest.x+size+10), (dest.y+5));
	}
}


void Checkbox::set_x(int x) {
    this->x = x;
}

void Checkbox::set_y(int y) {
    this->y = y;
}
