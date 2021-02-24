#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stack>

#include "glad.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>

#include "timer.hpp"
#include "config.hpp"
#include "constants.hpp"
#include "texturehandler.hpp"
#include "gameclient.hpp"
#include "menu.hpp"
#include "winsize.hpp"
#include "eventwrapper.hpp"

enum GAME_STATE {
	// Menu
	STATE_MAIN_MENU,

	// Game
	STATE_PLAYING,
};

struct Game
{
	Game(Timer& timer, int argc, char* argv[]);
	~Game();

	void init(bool fullscreen);
	void gameInit();
	void eventHandler();
	void render();
	void update();
	void free();

    bool isRunning = false;
private:
    int argc;
    char** argv;
	const char* title;
	
	bool hasFreed = false;

    std::stack<int> state;
    std::shared_ptr<GameClient> game;
    std::shared_ptr<Menu> menu;

    WinSize winSize;

    Timer& timer;
    EventWrapper events;
    SDL_Window* window;
    SDL_GLContext context;
    std::shared_ptr<TextureHandler> textures;
};
