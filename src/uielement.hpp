#ifndef UIELEMENT_HPP
#define UIELEMENT_HPP

#include <iostream>
#include "eventhandler.hpp"

class UiElement {
public:
	UiElement();
	virtual ~UiElement();

	// on_change functions
	virtual void on_change(void (*function)(int, int));
	virtual bool is_changed();
	// Getters
	virtual int get_id();
	virtual void get_value(bool&);
	//virtual void get_value_string(std::string&) {}
	// Setters
	virtual void set_x(int x);
	virtual void set_y(int y);
	// Update functions
	virtual void update();
	virtual void update(EventHandler& events);
	virtual void update(EventHandler& events, int x_offset, int y_offset);
	// Render functions
	virtual void render();
	virtual void render(SDL_Renderer* renderer);
	virtual void render(SDL_Renderer* renderer, int x_offset, int y_offset);
};

#endif
