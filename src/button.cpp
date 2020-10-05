#include "button.hpp"

Button::Button(int id, SDL_Renderer* renderer,
        TextureHandler &textures,
        int x,
        int y, 
        int width, 
        int height) 
    : id{static_cast<unsigned int>(id)}, width{width}, height{height}, hovered{false}, button_text{nullptr} {
    this->renderer = renderer;
    this->textures = &textures;
    this->x = x;
    this->y = y;
}

Button::~Button() {}

void Button::update(int mouse_x, int mouse_y, int mouse_state) {
    src.h = 16;
    src.w = 16;
    src.x = 0;

    // Change sprite to hovered version if hovered is true
    src.y = (static_cast<int>(hovered))*16;

    dest.h = height;
    dest.w = width-(32*2);
    dest.x = x+32;
    dest.y = y;

    if (clicked == true) {
        clicked = false;
    }

    int calc_width = dest.w + (src.w*2)*2;
    if (AABB(mouse_x, mouse_y, 1, 1, x, y, calc_width, dest.h)) {
        hovered = true;
        if (mouse_state == 1) {
            dest.y += 5;
            being_clicked = true;
        } else {
            if (being_clicked == true) {
                clicked = true;
                being_clicked = false;
            }
        }

    } else {
        hovered = false;
        being_clicked = false;
        clicked = false;
    }
}

bool Button::get_pressed() {
    return clicked;
}

int Button::get_id() {
    return id;
}

void Button::render() {
    SDL_Rect begin{x, dest.y, src.w*2, dest.h}, end{dest.x+dest.w, dest.y, src.w*2, dest.h};
    SDL_RenderCopy(renderer, textures->get_texture(6), &src, &begin);
    SDL_RenderCopy(renderer, textures->get_texture(5), &src, &dest);
    SDL_RenderCopy(renderer, textures->get_texture(7), &src, &end);
    if (button_text != nullptr) {
        button_text->draw(dest.x, dest.y);
    }
}

void Button::set_text(std::string text) {
    this->text = text;
    SDL_Color color;
    color.r = 255;
    color.g = 255;
    color.b = 255;
    color.a = 255;
    
    button_text = std::shared_ptr<Text>(new Text(renderer, this->text, color, constants::button_font));
}

void Button::set_x(int x) {
    this->x = x;
}

void Button::set_y(int y) {
    this->y = y;
}

std::string Button::get_text() {
    return text;
}
