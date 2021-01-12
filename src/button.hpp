#pragma once

#include <iostream>
#include <string>
#include <memory>

#include <SDL2/SDL.h>

#include "generic.hpp"
#include "eventwrapper.hpp"
#include "texturehandler.hpp"
#include "uielement.hpp"
#include "constants.hpp"
#include "text.hpp"

struct Button: public UiElement
{
	Button() = default;
	Button(SDL_Renderer* renderer, unsigned int id, int x, int y, int width, int height = 32);

	void setX(int x);
	void setY(int y);
    
	void update(EventWrapper& events, int offsetX = 0, int offsetY = 0);
	void render(SDL_Renderer* renderer, TextureHandler& textures, int offsetX = 0, int offsetY = 0);

	// Text
	void setText(std::string text);
    
    std::string text;
    bool clicked;
private:	
	int x;
	int y;
	int width;
	int height;

	// Mouse state
    bool beingClicked;
	bool hovered;

	SDL_Rect src, dest;
    std::unique_ptr<Text> buttonText;
};
