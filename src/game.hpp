#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>

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

struct Game
{
	Game();
	~Game();

	void init(bool fullscreen);
	void gameInit();
	void eventHandler();
	void render();
	void update();
	void free();

    bool isRunning = false;
private:
	const char* title;
	
	bool hasFreed = false;

	State state;
    std::shared_ptr<Play> game;
    std::shared_ptr<Menu> menu;

    WinSize winSize;

    EventWrapper events;
    SDL_Window* window;
	SDL_Renderer* renderer;
    std::shared_ptr<TextureHandler> textures;
};
