#ifndef BUTTON_HPP
#define BUTTON_HPP
#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "uielement.hpp"
#include "constants.hpp"
#include "texturehandler.hpp"
#include "text.hpp"
#include "aabb.hpp"

class Button: public UiElement {
public:
    Button() = default;
    Button(int id, SDL_Renderer* renderer, TextureHandler &textures, int x, int y, int width, int height = 32);
    ~Button();

    void set_x(int x);
    void set_y(int y);
    void update(int mouse_x, int mouse_y, int mouse_state);
    void render();

    // Text
    void set_text(std::string text);
    std::string get_text();
    bool get_pressed();
    int get_id();
private:
    SDL_Renderer* renderer;
    TextureHandler* textures;
    std::string text;
    unsigned int id;
    int x;
    int y;
    int width;
    int height;

    // Mouse state
    bool hovered;
    bool being_clicked;
    bool clicked;

    SDL_Rect src, dest;
    Text* button_text;

};

#endif // BUTTON_HPP
