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
#include "chunkgen.hpp"
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
#include "timer.hpp"

struct Play
{
	Play(SDL_Renderer* renderer, TextureHandler& textures, EventWrapper& events, Timer& timer, WinSize& winSize);
	~Play();

	void render();
	void update();
private:
    WinSize& winSize;
	void handleCamera();
	void drawSelection(int* p1, int* p2);
	void deadParticles();
	void mouseEvents();
	
    SDL_Renderer* renderer;
    TextureHandler& textures;
	EventWrapper& events;
    std::shared_ptr<ChunkGen> generator;
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
    Timer timer;

    std::shared_ptr<Background> background;
	std::vector<Structure*> structures;
};
