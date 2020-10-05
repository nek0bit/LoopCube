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
#include "animation.hpp"
#include "inventory.hpp"
#include "text.hpp"
#include "gravityparticle.hpp"
#include "tree.hpp"

class Play {

public:
    Play() {};
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
    //void draw_debug_menu();

    SDL_Renderer* renderer;
    TextureHandler* textures;
    EventHandler* events;
    Chunk_Group chunks;
	std::unique_ptr<Inventory> inv;
    Camera camera;
    Player player;

    // Animations
    Animation fade;
    std::vector<GravityParticle> particles;

    std::vector<Structure*> structures;
};


#endif // PLAY_HPP
