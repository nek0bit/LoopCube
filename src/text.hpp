#ifndef TEXT_HPP
#define TEXT_HPP
#include <iostream>
#include <string>
#include <array>

#include <SDL2/SDL.h>

struct Text
{
	Text(std::string text = "");
	~Text();

	void render(SDL_Renderer* renderer, int x, int y, int size = 10);
	void operator=(const std::string& value);
	void set_text(std::string& text);
    
	int width;
	int height;
private:
	std::vector<int> char_width;
	std::string text;

};

#endif // TEXT_HPP
