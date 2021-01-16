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
    int scroll; // 0 = null, -1 = down, 1 = up
};

struct EventWrapper
{
    EventWrapper();
	~EventWrapper();

	void updateControllers();
	
	void listen();

	// Text mode
    bool textMode;    
	std::string textModeBuffer;

	std::vector<int> buttonMapping; // Maps to proper state index
	std::vector<int> buttonState;
	std::vector<int> keyMapping; // Maps to proper state index
	std::vector<int> keyState;

    bool quit; // If true, game should quit entirely
    
	// vmouse = virtual mouse; Could be a real mouse, could be emulated (via a controller or touch screen)
    // Also, vmouse_down/vmouse_clicked must have 0: none; 1: left click; 2: middle click; 3: right click
    VMouse vmouse;
    
private:
    void resizeInputStates();
    
	SDL_Joystick* controller;
	SDL_Event event;
};
