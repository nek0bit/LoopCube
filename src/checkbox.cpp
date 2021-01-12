#include "checkbox.hpp"

Checkbox::Checkbox(SDL_Renderer* renderer, int id, std::string text, int x, int y, int size, bool checked)
	: UiElement{},
      changed{false},
      text{text},
      x{x},
      y{y},
      size{size},
      checked{checked},
      text_render{nullptr}
{
    this->id = id;
    
    SDL_Color color{255, 255, 255, 255};
    text_render = new Text{renderer, text, color, constants::fontHandler.getFont(2)};
}

Checkbox::~Checkbox()
{
    delete text_render;
}

void Checkbox::onChange(void (*function)(int, int))
{
	if (changed) {
		(*function)(id, checked ? 1 : 0);
	}
}

void Checkbox::getValue(bool& here)
{
	here = checked;
}

bool Checkbox::isChanged()
{
	return changed;
}

bool Checkbox::toggle()
{
	checked = !checked;
	return checked;
}

void Checkbox::update(EventWrapper& events, int offset_x, int offset_y)
{
	// Clear changed
	changed = false;

	if (events.vmouse.clicked &&
		Generic::collision<int>(offset_x+x, offset_y+y, size, size, events.vmouse.x, events.vmouse.y, 1, 1))
    {
		toggle();
		changed = true;
	}

	dest.x = x;
	dest.y = y;
	dest.w = size;
	dest.h = size;
}

void Checkbox::render(SDL_Renderer* renderer, TextureHandler& textures, int offset_x, int offset_y)
{
	SDL_Rect mod_dest{offset_x+dest.x, offset_y+dest.y, dest.w, dest.h};
	// Draw box behind check
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
    SDL_RenderFillRect(renderer, &mod_dest);

	// Draw check
	if (checked)
    {
	    SDL_Rect check{mod_dest.x+10, mod_dest.y+10, mod_dest.w-20, mod_dest.h-20};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &check);
	}

	text_render->draw(mod_dest.x+size+10, mod_dest.y+5);
}


void Checkbox::setX(int x)
{
	this->x = x;
}

void Checkbox::setY(int y)
{
	this->y = y;
}
