#pragma once
#include <iostream>
#include <string>
#include <memory>

#include <SDL2/SDL.h>

#include "generic.hpp"
#include "uielement.hpp"
#include "text.hpp"
#include "constants.hpp"

struct Checkbox: public UiElement
{
	Checkbox() = default;
	Checkbox(SDL_Renderer* renderer, int id, std::string text, int x, int y, int size, bool checked=false);
    ~Checkbox() = default;

	void onChange(void (*function)(int, int)) override;
	void getValue(bool& here) override;
	bool isChanged() override;
    
	bool toggle();
    bool checked;

	void setX(int x) override;
	void setY(int y) override;
	void update(EventWrapper& events, Timer& timer, int offsetX = 0, int offsetY = 0) override;
	void render(SDL_Renderer* renderer, TextureHandler& textures, int offsetX = 0, int offsetY = 0) override;
private:
	bool changed;
	std::string text;
	int x;
	int y;
	int size;

    std::shared_ptr<Text> textRender;

    SDL_Rect dest;
};
