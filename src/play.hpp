#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <cmath>
#include <ctime>
#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "winsize.hpp"
#include "eventwrapper.hpp"
#include "constants.hpp"
#include "texturehandler.hpp"
#include "camera.hpp"
#include "chunkgroup.hpp"
#include "player.hpp"
#include "testentity.hpp"
#include "animation.hpp"
#include "inventory.hpp"
#include "text.hpp"
#include "gravityparticle.hpp"
#include "tree.hpp"
#include "background.hpp"
#include "background_overworld.hpp"
#include "time.hpp"

struct Play
{
	Play(SDL_Renderer* renderer, TextureHandler& textures, EventWrapper& events, WinSize& winSize);
	~Play();

	void event_handler();
	void render();
	void update();
private:
    WinSize& winSize;
	void handle_camera();
	void draw_selection(int* p1, int* p2);
	void dead_particles();
	void update_config();
	void mouse_events();

	// Options
	bool show_particles;
	
    SDL_Renderer* renderer;
    TextureHandler& textures;
	EventWrapper& events;
	ChunkGroup chunks;
	std::unique_ptr<Inventory> inv;
	Camera camera;

	// Entities
	Player player;
	std::vector<Entity*> entities;

	// Animations
	Animation fade;
	std::vector<GravityParticle> particles;

	Time time;

    std::shared_ptr<Background> background;
	std::vector<Structure*> structures;
};
