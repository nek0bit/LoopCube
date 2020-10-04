#include "text.hpp"

Text::Text(SDL_Renderer* renderer,
        std::string text, 
        SDL_Color color,
        TTF_Font* font)
    :  text{text}, color{color}, renderer{renderer} {
    this->font = font;

    if (this->font == NULL) {
        std::cerr << "[ERROR] TTF_OpenFont: " << TTF_GetError() << std::endl;
    }
    #ifdef __LOWENDDEVICE__
        surface = TTF_RenderText_Solid(this->font, text.c_str(), color);
    #else
        surface = TTF_RenderText_Blended(this->font, text.c_str(), color);
    #endif
    messageText = SDL_CreateTextureFromSurface(renderer, surface);
}

Text::~Text() {
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(messageText);
}

void Text::set_text(std::string text) {
    this->text = text;
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(messageText);
    surface = TTF_RenderText_Blended(this->font, text.c_str(), color);
    messageText = SDL_CreateTextureFromSurface(renderer, surface);
}

void Text::draw(int x, int y) {
    int w, h;
    TTF_SizeText(font, text.c_str(), &w, &h);
    SDL_Rect pos{x, y, w, h};
    SDL_RenderCopy(renderer, messageText, NULL, &pos);
}
