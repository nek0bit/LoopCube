#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "config.hpp"
#include "constants.hpp"
#include "texturehandler.hpp"
#include "play.hpp"
#include "menu.hpp"
#include "state.hpp"
#include "winsize.hpp"
#include "eventwrapper.hpp"

class Game {

public:
	Game();
	~Game();

	void init(bool fullscreen);
	void game_init();
	void event_handler();
	void render();
	void update();
	bool running();
	void free();
private:
	const char* title;
	
	bool has_freed = false;
	bool is_running = false;

	State state;
	Play* game;
	Menu* menu;

    WinSize winSize;

    EventWrapper events;
    SDL_Window* window;
	SDL_Renderer* renderer;
    TextureHandler* textures;
};
