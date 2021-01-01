#include "checkbox.hpp"

Checkbox::Checkbox(int id, std::string text, int x, int y, int size, bool checked)
	: changed{false}, id{id}, text{text}, x{x}, y{y}, size{size}, checked{checked}, text_render{text} {
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

void Checkbox::update(EventWrapper*& events, int offset_x, int offset_y) {
	// Clear changed
	changed = false;

	if (events->mouse.clicked &&
		AABB(offset_x+x, offset_y+y, size, size, pos[0], pos[1], 1, 1)) {
		toggle();
		changed = true;
	}

	dest.x = x;
	dest.y = y;
	dest.w = size;
	dest.h = size;
}

void Checkbox::render(GraphicsWrapper* renderer, int offset_x, int offset_y) {
	SDL_Rect mod_dest{offset_x+dest.x, offset_y+dest.y, dest.w, dest.h};
	// Draw box behind check
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
    SDL_RenderFillRect(renderer, &mod_dest);

	// Draw check
	if (checked) {
	    SDL_Rect check{mod_dest.x+10, mod_dest.y+10, mod_dest.w-20, mod_dest.h-20};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &check);
	}

	text_render.render(renderer, mod_dest.x+size+10, mod_dest.y+5, 10);
}


void Checkbox::set_x(int x) {
	this->x = x;
}

void Checkbox::set_y(int y) {
	this->y = y;
}
