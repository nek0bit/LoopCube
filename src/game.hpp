#ifndef GAME_HPP
#define GAME_HPP
#include <iostream>
#include <string>
#include <vector>

#include "backendincludes.hpp"

#include "constants.hpp"
#include "texturehandler.hpp"
#include "play.hpp"
#include "menu.hpp"
#include "state.hpp"

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

	GraphicsWrapper* renderer;
	EventWrapper* events;
};


#endif // GAME_HPP
