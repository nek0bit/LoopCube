#ifndef INVENTORY_HPP
#define INVENTORY_HPP
#include <iostream>
#include <vector>
#include <algorithm>

#include <SDL2/SDL.h>

#include "texturehandler.hpp"
#include "winsize.hpp"
#include "transition.hpp"
#include "constants.hpp"
#include "position.hpp"
#include "item.hpp"

class Inventory {
public:
	Inventory() = default;
	Inventory(SDL_Renderer* renderer,
              TextureHandler* textures,
			  EventWrapper& events);
	~Inventory();

	void draw_hotbar();
	void draw_inventory_menu();
	bool get_inventory_visibility();
	Item& get_selected_item(); 
	void add_item(int id);
	void update();
private:
    WinSize& winSize;
	bool animation;
	int hotbar_slots;
	int max_slots;
	bool visible;
	bool show_inventory_menu;
	int hotbar_pos;
	std::vector<int> get_hovered_pos(int x, int y, int corner_x, int corner_y, bool draw);
	bool collision(int, int, int, int, int, int, int, int);

	std::vector<int> slots;

	std::vector<Item> items;
	
	// Temporary item slot for holding items
	Item item_held;

	GraphicsWrapper* renderer;
    TextureHandler* textures;
	EventWrapper* events;

    Transition inventory_slide;
	Transition hotbar_slide;
};

#endif // INVENTORY_HPP
