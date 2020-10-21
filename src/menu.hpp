#ifndef MENU_HPP
#define MENU_HPP
#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include <cmath>
#include <random>
#include <memory>

#include "textbox.hpp"
#include "checkbox.hpp"
#include "texturehandler.hpp"
#include "eventhandler.hpp"
#include "constants.hpp"
#include "button.hpp"
#include "animation.hpp"
#include "item.hpp"

class Menu {
public:
	Menu(SDL_Renderer* renderer, 
		 TextureHandler &textures,
		 EventHandler &events,
		 int* WINDOW_W,
		 int* WINDOW_H);
	~Menu();

	void update(bool update_animations = true);
	void render();
	void render_background();
	void render_sidebar();
	void render_main_menu();
	void render_config_menu();
	void set_state(int state);
	int get_pressed();
private:
	// Functions
	static void update_config_elements(int id, int value);
	
	int state;
	// Main Menu Stuff
	std::vector<std::string> option_str;
	std::vector<int> option_state; 
	std::vector<std::unique_ptr<Button>> button_group;

	// MISC
	const int BLOCK_S;
	const int BUTTON_W;
	int* WINDOW_W;
	int* WINDOW_H;

	// Config stuff
	std::unique_ptr<Button> back_button;
	std::vector<UiElement*> c_elements;
	
	// Important setup
	SDL_Renderer* renderer;
	TextureHandler* textures;
	EventHandler* events;
	Animation shift;

	// Content elements for sidebar
	Item random_block;
	std::unique_ptr<Text> header;
	std::unique_ptr<Text> paragraph;
	std::string p_string;
	const int pad_left;
};


#endif // MENU_HPP
