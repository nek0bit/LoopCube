#ifndef UIELEMENT_HPP
#define UIELEMENT_HPP

#include <iostream>
#include "eventhandler.hpp"

class UiElement {
public:
	UiElement();
	virtual ~UiElement();

	virtual void set_x(int x);
	virtual void set_y(int y);
	virtual void update();
	virtual void update(EventHandler& events);
	virtual void update(EventHandler& events, int x_offset, int y_offset);
	virtual void render();
	virtual void render(SDL_Renderer* renderer);
	virtual void render(SDL_Renderer* renderer, int x_offset, int y_offset);
};

#endif
