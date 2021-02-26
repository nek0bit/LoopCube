#pragma once

#include <iostream>
#include <string>
#include <memory>

#include "generic.hpp"
#include "eventwrapper.hpp"
#include "texturehandler.hpp"
#include "uielement.hpp"
#include "constants.hpp"
#include "text.hpp"

struct Button: public UiElement
{
	Button() = default;
	Button(const Graphics& renderer, unsigned int id, int x, int y, int width, int height = 32);

	void setX(int x) override;
	void setY(int y) override;
    
	void update(EventWrapper& events, Timer& timer, int offsetX = 0, int offsetY = 0) override;
	void render(const Graphics& graphics, TextureHandler& textures, int offsetX = 0, int offsetY = 0) override;

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
