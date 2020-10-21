#ifndef PLAY_HPP
#define PLAY_HPP

#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <cmath>
#include <ctime>

#include "constants.hpp"
#include "texturehandler.hpp"
#include "camera.hpp"
#include "eventhandler.hpp"
#include "chunkgroup.hpp"
#include "player.hpp"
#include "testentity.hpp"
#include "animation.hpp"
#include "inventory.hpp"
#include "text.hpp"
#include "gravityparticle.hpp"
#include "tree.hpp"
#include "background.hpp"

class Play {

public:
	Play(SDL_Renderer* renderer, TextureHandler &textures, EventHandler &events, int *WINDOW_W, int *WINDOW_H);
	~Play();

	void event_handler();
	void render();
	void update();
private:
	int *WINDOW_W;
	int *WINDOW_H;
	void handle_camera();
	void draw_selection(int* p1, int* p2);
	void print_mouse_pos();
	void dead_particles();
	void update_config();
	void mouse_events();
	//void draw_debug_menu();

	// Options
	bool show_particles;
	
	SDL_Renderer* renderer;
	TextureHandler& textures;
	EventHandler& events;
	Chunk_Group chunks;
	std::unique_ptr<Inventory> inv;
	Camera camera;

	// Entities
	Player player;
	std::vector<Entity*> entities;

	// Animations
	Animation fade;
	std::vector<GravityParticle> particles;

	Background background;
	std::vector<Structure*> structures;
};


#endif // PLAY_HPP
