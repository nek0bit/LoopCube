#ifndef CHECKBOX_HPP
#define CHECKBOX_HPP
#include <iostream>

#include <SDL2/SDL.h>

#include "uielement.hpp"
#include "text.hpp"
#include "aabb.hpp"
#include "constants.hpp"

struct Checkbox: public UiElement
{
	Checkbox() = default;
	Checkbox(SDL_Renderer* renderer, int id, std::string text, int x, int y, int size, bool checked=false);
	~Checkbox();

	void on_change(void (*function)(int, int));
	int get_id();
	void get_value(bool& here);
	bool is_changed();
	bool toggle();
	bool get_checked();
	void check();
	void uncheck();

	void set_x(int x);
	void set_y(int y);
	void update(EventWrapper*& events, int offset_x = 0, int offset_y = 0);
	void render(SDL_Renderer* renderer, TextureHandler& textures, int offset_x = 0, int offset_y = 0);
private:
	bool changed;
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
