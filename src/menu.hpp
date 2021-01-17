#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <random>
#include <memory>

#include "timer.hpp"
#include "generic.hpp"
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

struct Menu
{
	Menu(SDL_Renderer* renderer, TextureHandler& textures, EventWrapper& events, Timer& timer, WinSize& winSize);
	~Menu();

	void update(bool update_animations = true);
	void render();
	void renderBackground();
	void renderSidebar();
	void renderMainMenu();
	void renderConfigMenu();
	void setState(int state);
	int getPressed();

    bool showPlayBuffer;
private:
	// Functions
	static void updateConfigElements(int id, int value);
	
	int state;

    WinSize& winSize;
    Timer& timer;
	
	int offsetX;
	int offsetY;
	// Main Menu Stuff
	std::vector<std::string> optionStr;
	std::vector<int> optionState; 
	std::vector<std::unique_ptr<Button>> buttonGroup;

	// MISC
	const int BUTTON_W;

    // Background movement
    float bgX;
    float bgY;

	// Config stuff
	std::unique_ptr<Button> backButton;
	std::vector<UiElement*> cElements;
	
	// Important setup
	SDL_Renderer* renderer;
    TextureHandler& textures;
    EventWrapper& events;
	Animation shift;

	Box back;
	const int boxWidth;
	const int boxHeight;
	
	// Content elements for sidebar
	int prevMidLeft;
	int midLeft;
	int contentLeft;
	
	Item randomBlock;
    std::string pString;

	Text* header;
	Text* paragraph;
	const int padLeft;
};
