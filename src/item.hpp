#pragma once
#include <iostream>
#include <string>
#include <memory>

#include <SDL2/SDL.h>

#include "text.hpp"
#include "constants.hpp"
#include "blockinfo.hpp"
#include "texturehandler.hpp"

struct Item {
	Item();
	Item(SDL_Renderer* renderer, int id);
	~Item() = default;

	void render(SDL_Renderer* renderer, TextureHandler& textures, int x, int y, int width, int height);
	void addCount(int amount = 1);
    int count;
    BlockInfo block;
	bool enabled;
private:
    std::shared_ptr<Text> text;
	std::string id;
};
