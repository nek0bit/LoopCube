#include "text.hpp"

Text::Text(SDL_Renderer* renderer,
        std::string text, 
        SDL_Color color,
		TTF_Font* font,
		int wrap_width)
    :  wrap_width{wrap_width}, text{text}, color{color}, renderer{renderer} {
    this->font = font;

    if (this->font == NULL) {
        std::cerr << "[ERROR] TTF_OpenFont: " << TTF_GetError() << std::endl;
    }
	update_surface();
	messageText = SDL_CreateTextureFromSurface(renderer, surface);
}

Text::~Text() {
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(messageText);
}

void Text::update_surface() {
	if (wrap_width == -1) {
		surface = TTF_RenderText_Blended(this->font, text.c_str(), color);
	} else {
		surface = TTF_RenderText_Blended_Wrapped(this->font, text.c_str(), color, wrap_width);
	}
}

void Text::set_text(std::string text) {
    this->text = text;
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(messageText);
	update_surface();
	messageText = SDL_CreateTextureFromSurface(renderer, surface);
}

int Text::get_width() {
	if (surface != nullptr) return surface->w;
	return 0;
}

int Text::get_height() {
	if (surface != nullptr) return surface->h;
	return 0;
}

void Text::draw(int x, int y) {
    int w = 0, h = 0;
	if (surface != nullptr) {
		w = surface->w;
		h = surface->h;
	}
	SDL_Rect pos{x, y, w, h};
    SDL_RenderCopy(renderer, messageText, NULL, &pos);
}
