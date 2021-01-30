#include "button.hpp"

Button::Button(SDL_Renderer* renderer,
               unsigned int id,
			   int x,
			   int y, 
			   int width, 
			   int height) 
	: UiElement{},
      clicked{false},
      width{width},
      height{height},
      beingClicked{false},
      hovered{false},
      buttonText{nullptr}
{
    this->id = id;
	this->x = x;
	this->y = y;

    SDL_Color fontColor{244, 244, 244, 255};
    buttonText = std::make_unique<Text>(Text(renderer, text, fontColor, constants::fontHandler.getFont(4)));
}

void Button::update(EventWrapper& events, Timer& timer, int offsetX, int offsetY) {
	src.x = 0;
	// Change sprite to hovered version if hovered is true
	src.y = static_cast<int>(hovered) * 16;
    src.w = 16;
    src.h = 16;

	dest.x = x+32;
	dest.y = y;
    dest.w = width - (32 * 2);
    dest.h = height;

    if (clicked == true)
    {
        clicked = false;
        beingClicked = false;
    }
    
	const int calcWidth = dest.w + (src.w*2)*2;

	if (Generic::collision<int>(events.vmouse.x, events.vmouse.y, 1, 1, offsetX + x, offsetY + y, calcWidth, dest.h))
    {
		hovered = true;
		if (events.vmouse.down == 1)
        {
			dest.y += 5;
            beingClicked = true;
		}
        else if (beingClicked)
        {
            clicked = true;
            beingClicked = true;
        }
    }
    else
    {
		hovered = false;
        beingClicked = false;
		clicked = false;
	}
}

void Button::render(SDL_Renderer* renderer, TextureHandler& textures, int offsetX, int offsetY)
{
	SDL_Rect begin{offsetX+x, offsetY+dest.y, src.w*2, dest.h},
		end{offsetX+dest.x+dest.w, offsetY+dest.y, src.w*2, dest.h};
	SDL_Rect mod_dest{offsetX+dest.x, offsetY+dest.y, dest.w, dest.h};

    SDL_RenderCopy(renderer, textures.getTexture(TEXTURE_MENU_SOLID_LEFT)->texture, &src, &begin);
    SDL_RenderCopy(renderer, textures.getTexture(TEXTURE_MENU_SOLID)->texture, &src, &mod_dest);
    SDL_RenderCopy(renderer, textures.getTexture(TEXTURE_MENU_SOLID_RIGHT)->texture, &src, &end);
    
	if (buttonText != nullptr)
		buttonText->draw(x+(width/2)-(buttonText->getWidth()/2)-2,
							mod_dest.y+(height/2)-(buttonText->getHeight()/2)-2);
}

void Button::setText(std::string text)
{
	this->text = text;
    buttonText->setText(text);
}

// Exists for compatibility with UiElement class
void Button::setX(int x)
{
	this->x = x;
}

void Button::setY(int y)
{
	this->y = y;
}
