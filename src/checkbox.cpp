#include "checkbox.hpp"

Checkbox::Checkbox(SDL_Renderer* renderer, int id, std::string text, int x, int y, int size, bool checked)
	: UiElement{},
      checked{checked},
      changed{false},
      text{text},
      x{x},
      y{y},
      size{size},
      textRender{nullptr}
{
    this->id = id;
    
    SDL_Color color{255, 255, 255, 255};
    textRender = std::shared_ptr<Text>(new Text{renderer, text, color, constants::fontHandler.getFont(2)});
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

void Checkbox::update(EventWrapper& events, Timer& timer, int offsetX, int offsetY)
{
	// Clear changed
	changed = false;

	if (events.vmouse.clicked &&
		Generic::collision<int>(offsetX+x, offsetY+y, size, size, events.vmouse.x, events.vmouse.y, 1, 1))
    {
		toggle();
		changed = true;
	}

	dest.x = x;
	dest.y = y;
	dest.w = size;
	dest.h = size;
}

void Checkbox::render(SDL_Renderer* renderer, TextureHandler& textures, int offsetX, int offsetY)
{
	SDL_Rect mod_dest{offsetX+dest.x, offsetY+dest.y, dest.w, dest.h};
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

	textRender->draw(mod_dest.x+size+10, mod_dest.y+5);
}


void Checkbox::setX(int x)
{
	this->x = x;
}

void Checkbox::setY(int y)
{
	this->y = y;
}
