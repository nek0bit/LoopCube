#pragma once

#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#

struct Text
{
	Text(SDL_Renderer* renderer,
		 std::string text,
		 SDL_Color color,
		 TTF_Font* font,
		 int wrapWidth = -1);
	~Text();

	int getWidth();
	int getHeight();
	void draw(int x, int y);
	void setText(std::string text);
private:
	void updateSurface();
	int wrapWidth;
	std::string text;

	SDL_Color color;
	SDL_Renderer* renderer;
	TTF_Font* font;
	SDL_Surface* surface;
	SDL_Texture* messageText;

};
