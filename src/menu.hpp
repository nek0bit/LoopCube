#ifndef MENU_HPP
#define MENU_HPP
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <random>
#include <memory>

#include "box.hpp"
#include "texturehandler.hpp"
#include "winsize.hpp"
#include "text.hpp"
#include "textbox.hpp"
#include "checkbox.hpp"
#include "constants.hpp"
#include "button.hpp"
#include "animation.hpp"
#include "item.hpp"

class Menu {
public:
	Menu(SDL_Renderer* renderer, TextureHandler& textures, EventWrapper& events, WinSize& winSize);
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

    WinSize& winSize;
	
	int offset_x;
	int offset_y;
	// Main Menu Stuff
	std::vector<std::string> option_str;
	std::vector<int> option_state; 
	std::vector<std::unique_ptr<Button>> button_group;

	// MISC
	const int BLOCK_S;
	const int BUTTON_W;

	// Config stuff
	std::unique_ptr<Button> back_button;
	std::vector<UiElement*> c_elements;
	
	// Important setup
	SDL_Renderer* renderer;
    TextureHandler& textures;
    EventWrapper& events;
	Animation shift;

	Box back;
	const int box_width;
	const int box_height;
	
	// Content elements for sidebar
	int prev_mid_left;
	int mid_left;
	int content_left;
	
	Item random_block;
	Text* header;
	Text* paragraph;
	std::string p_string;
	const int pad_left;
};


#endif // MENU_HPP
