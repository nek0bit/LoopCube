#include "text.hpp"

Text::Text(const Graphics& graphics,
		   std::string text, 
		   SDL_Color color,
		   TTF_Font* font,
		   int wrapWidth)
	:  wrapWidth{wrapWidth},
       text{text},
       color{color}
{
	this->font = font;

	if (this->font == NULL) {
		std::cerr << "[Error] TTF_OpenFont: " << TTF_GetError() << ".. expect dragons and segfaults" << std::endl;
	}
	updateSurface();
	//messageText = SDL_CreateTextureFromSurface(renderer, surface);
}

Text::~Text()
{
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(messageText);
}

void Text::updateSurface()
{
	if (wrapWidth == -1) {
		surface = TTF_RenderText_Blended(this->font, text.c_str(), color);
	} else {
		surface = TTF_RenderText_Blended_Wrapped(this->font, text.c_str(), color, wrapWidth);
	}
}

void Text::setText(const Graphics& graphics, std::string text)
{
	this->text = text;
    SDL_FreeSurface(surface);
	SDL_DestroyTexture(messageText);
    updateSurface();
	//messageText = SDL_CreateTextureFromSurface(renderer, surface);
}

int Text::getWidth()
{
	if (surface != nullptr) return surface->w;
	return 0;
}

int Text::getHeight()
{
	if (surface != nullptr) return surface->h;
	return 0;
}

void Text::draw(const Graphics& graphics, int x, int y)
{
	int w = 0, h = 0;
	if (surface != nullptr) {
		w = surface->w;
		h = surface->h;
	}
	SDL_Rect pos{x, y, w, h};
	//SDL_RenderCopy(renderer, messageText, NULL, &pos);
}
