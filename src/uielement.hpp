#pragma once

#include <iostream>

#include <SDL2/SDL.h>

#include "texturehandler.hpp"
#include "eventwrapper.hpp"

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
	virtual void update(EventWrapper& events);
	virtual void update(EventWrapper& events, int offsetX, int offsetY);
	// Render functions
	virtual void render();
	virtual void render(SDL_Renderer* renderer, TextureHandler& textures);
	virtual void render(SDL_Renderer* renderer, TextureHandler& textures, int offsetX, int offsetY);

    unsigned int id;
};

