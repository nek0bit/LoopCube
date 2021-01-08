#pragma once

#include <iostream>
#include <vector>
#include <array>

#include <SDL2/SDL.h>

struct VMouse
{
    int x;
    int y;
    int down;
    int clicked;
};

struct EventWrapper
{
    EventWrapper();
	~EventWrapper();

	void update_controllers();
	
	void listen();

	// Text mode
	void enable_text_mode();
	void disable_text_mode();
	void set_text_mode_buffer(std::string str);
	void clear_text_mode_buffer();
    
	std::string text_mode_buffer;

	std::vector<int> button_mapping; // Maps to proper state index
	std::vector<int> button_state;
	std::vector<int> key_mapping; // Maps to proper state index
	std::vector<int> key_state;

    bool quit; // If true, game should quit entirely
	// vmouse = virtual mouse; Could be a real mouse, could be emulated (via a controller or touch screen)
    VMouse vmouse;
private:
	SDL_Joystick* controller;
	SDL_Event event;

    


	// Also, vmouse_down/vmouse_clicked must have 0: none; 1: left click; 2: middle click; 3: right click
    
	bool text_mode;
};
