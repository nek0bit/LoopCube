#pragma once
#include <iostream>

#include <SDL2/SDL.h>

#include "generic.hpp"
#include "uielement.hpp"
#include "text.hpp"
#include "constants.hpp"

struct Checkbox: public UiElement
{
	Checkbox() = default;
	Checkbox(SDL_Renderer* renderer, int id, std::string text, int x, int y, int size, bool checked=false);
	~Checkbox();

	void on_change(void (*function)(int, int)) override;
	int get_id() override;
	void get_value(bool& here) override;
	bool is_changed() override;
    
	bool toggle();
	bool get_checked();
	void check();
	void uncheck();

	void set_x(int x) override;
	void set_y(int y) override;
	void update(EventWrapper& events, int offset_x = 0, int offset_y = 0) override;
	void render(SDL_Renderer* renderer, TextureHandler& textures, int offset_x = 0, int offset_y = 0) override;
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
