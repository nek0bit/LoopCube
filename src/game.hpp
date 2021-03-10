#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stack>

#include "../include/glad/glad.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>

#include "model.hpp"
#include "graphics.hpp"
#include "timer.hpp"
#include "config.hpp"
#include "constants.hpp"
#include "texturehandler.hpp"
#include "gameclient.hpp"
#include "winsize.hpp"
#include "eventwrapper.hpp"
#include "generic.hpp"

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
    void handleFPS();
    void updateTitle();
	void eventHandler();
	void render();
	void update();
	void free();

    bool isRunning = false;
private:
    void createModels();
    glm::vec2 camZoom, camZoomRes;
    double tickTime;
    int argc;
    char** argv;
	const char* title;
	
	bool hasFreed = false;

    std::stack<int> state;
    std::shared_ptr<GameClient> game;

    WinSize winSize;

    Timer& timer;
    EventWrapper events;
    Graphics graphics;
};
