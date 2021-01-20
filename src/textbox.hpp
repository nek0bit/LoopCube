// TODO for v0.4, clean me up a lot
#pragma once
#include <iostream>
#include <string>

#ifdef __SWITCH__
#include <switch.h>
#endif

#include <SDL2/SDL.h>

#include "generic.hpp"
#include "eventwrapper.hpp"
#include "uielement.hpp"
#include "constants.hpp"
#include "text.hpp"
#include "animation.hpp"

class Textbox: public UiElement {
public:
	Textbox() = default;
	Textbox(int id, int x, int y, int width, int height);
	~Textbox();

	void update(EventWrapper& events, Timer& timer, int offset_x, int offset_y) override;
	void render(SDL_Renderer* renderer, TextureHandler& textures, int offset_x, int offset_y) override;
    
	int x;
	int y;
private:
	std::string text;
	int id;
	int width;
	int height;
	bool focused;
	Text* textbox_text;
	Animation blink;
	
#ifdef __SWITCH__
	Result rc;
	SwkbdConfig kbd;
#endif
	
	void handle_keyboard(EventWrapper& events);
};
