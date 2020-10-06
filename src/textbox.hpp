#ifndef TEXTBOX_HPP
#define TEXTBOX_HPP
#include <iostream>
#include <string>

#ifdef __SWITCH__
#include <switch.h>
#endif
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "uielement.hpp"
#include "constants.hpp"
#include "eventhandler.hpp"
#include "text.hpp"
#include "aabb.hpp"
#include "animation.hpp"

class Textbox: public UiElement {
public:
    Textbox() = default;
    Textbox(int id, int x, int y, int width, int height);
    ~Textbox();

	// TODO Add set_x, set_y functions
    void update(EventHandler& events);
    void render(SDL_Renderer* renderer);
private:
    std::string text;
    int id;
    int x;
    int y;
    int width;
    int height;
    bool focused;
    Text* textbox_text;
    Animation blink;
    
    #ifdef __SWITCH__
    Result rc;
    SwkbdConfig kbd;
    #endif
    
    void handle_keyboard(EventHandler& events);
};

#endif
