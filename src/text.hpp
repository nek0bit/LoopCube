#pragma once

#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "graphics.hpp"

struct Text
{
	Text(const Graphics& graphics,
		 std::string text,
		 SDL_Color color,
		 TTF_Font* font,
		 int wrapWidth = -1);
	~Text();

	int getWidth();
	int getHeight();
	void draw(const Graphics& graphics, int x, int y);
	void setText(const Graphics& graphics, std::string text);
private:
	void updateSurface();
	int wrapWidth;
	std::string text;

	SDL_Color color;
	TTF_Font* font;
	SDL_Surface* surface;
	SDL_Texture* messageText;

};
