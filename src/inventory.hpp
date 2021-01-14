#pragma once
#include <iostream>
#include <vector>
#include <algorithm>

#include <SDL2/SDL.h>

#include "generic.hpp"
#include "eventwrapper.hpp"
#include "texturehandler.hpp"
#include "winsize.hpp"
#include "transition.hpp"
#include "constants.hpp"
#include "position.hpp"
#include "item.hpp"

struct Inventory
{
	Inventory(SDL_Renderer* renderer,
              TextureHandler& textures,
			  EventWrapper& events,
              WinSize& winSize);
	~Inventory();

	void drawHotbar();
	void drawInventoryMenu();
    Item& getSelectedItem();
    bool showInventoryMenu;
	void addItem(int id);
	void update();
private:
    WinSize& winSize;
	bool animation;
	int hotbarSlots;
	int maxSlots;
	bool visible;
	int hotbarPos;
	std::vector<int> getHoveredPos(int x, int y, int cornerX, int cornerY, bool draw);

	std::vector<int> slots;

	std::vector<Item> items;
    Item itemHeld;
	
	SDL_Renderer* renderer;
    TextureHandler& textures;
	EventWrapper& events;

    Transition inventorySlide;
	Transition hotbarSlide;
};
