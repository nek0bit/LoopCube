#ifndef TEXT_HPP
#define TEXT_HPP
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class Text {
public:
	Text(SDL_Renderer* renderer,
		 std::string text,
		 SDL_Color color,
		 TTF_Font* font,
		 int wrap_width = -1);
	~Text();

	int get_width();
	int get_height();
	void draw(int x, int y);
	void set_text(std::string text);
private:
	void update_surface();
	int wrap_width;
	std::string text;

	SDL_Color color;
	SDL_Renderer* renderer;
	TTF_Font* font;
	SDL_Surface* surface;
	SDL_Texture* messageText;

};

#endif // TEXT_HPP
