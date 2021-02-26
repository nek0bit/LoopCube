#pragma once

#include <iostream>

#include "graphics.hpp"
#include "texturehandler.hpp"
#include "eventwrapper.hpp"
#include "timer.hpp"

struct UiElement
{
	UiElement();
	virtual ~UiElement();

	// on_change functions
	virtual void onChange(void (*function)(int, int));
	virtual bool isChanged();
	// Getters
	virtual void getValue(bool&);
	// Setters
	virtual void setX(int x);
	virtual void setY(int y);
	// Update functions
	virtual void update();
	virtual void update(EventWrapper& events, Timer& timer);
	virtual void update(EventWrapper& events, Timer& timer, int offsetX, int offsetY);
	// Render functions
	virtual void render();
	virtual void render(const Graphics& graphics, TextureHandler& textures);
	virtual void render(const Graphics& graphics, TextureHandler& textures, int offsetX, int offsetY);

    unsigned int id;
};

