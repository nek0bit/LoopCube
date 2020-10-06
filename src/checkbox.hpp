#ifndef CHECKBOX_HPP
#define CHECKBOX_HPP

#include <iostream>
#include <SDL2/SDL.h>

#include "uielement.hpp"
#include "eventhandler.hpp"
#include "text.hpp"
#include "aabb.hpp"
#include "constants.hpp"

class Checkbox: public UiElement {
public:
	Checkbox() = default;
	Checkbox(int id, std::string text, int x, int y, int size, bool checked=false);
	~Checkbox();

	bool toggle();
	bool get_checked();
	void check();
	void uncheck();

	void set_x(int x);
	void set_y(int y);
	void update(EventHandler& events);
	void render(SDL_Renderer* renderer);
	
private:
	int id;
	std::string text;
	int x;
	int y;
	int size;
	bool checked;

	Text* text_render;

	SDL_Rect dest;
};

#endif
