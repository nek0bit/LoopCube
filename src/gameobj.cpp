#include "gameobj.hpp"

Game_Object::Game_Object() {}

Game_Object::Game_Object(int texture_id, TextureHandler &textures, SDL_Renderer* renderer, Camera &camera,
    double x, double y, double w, double h) {
    obj.x = x;
    obj.y = y;
    obj.w = w;
    obj.h = h;

    this->texture_id = texture_id;
    this->renderer = renderer;
    // Camera needs to be a pointer incase another object changes the camera
    this->camera = &camera;
    this->textures = &textures;
}

Game_Object::~Game_Object() {
}

double Game_Object::get_default_x() const {
    return obj.x;
}

double Game_Object::get_default_y() const {
    return obj.y;
}

double Game_Object::get_x() const {
    return obj.x + (camera->get_x());
}

double Game_Object::get_y() const {
    return obj.y + (camera->get_y());
}

double Game_Object::get_width() const {
    return obj.w;
}

double Game_Object::get_height() const {
    return obj.h;
}

// Used for rendering optimizations
bool Game_Object::out_of_view() {
    if (get_x()+(obj.w*2) < 0 || get_y()+(obj.h*2) < 0) {
        return true;
    }
    if (get_x()-obj.w > camera->get_width() || get_y()-obj.h > camera->get_height()) {
        return true;
    }
    return false;
}

void Game_Object::update() {
    src.h = get_height();
    src.w = get_width();
    src.x = 0;
    src.y = 0;

    dest.h = src.h;
    dest.w = src.w;
    dest.x = get_x();
    dest.y = get_y();
}

void Game_Object::render() {
    SDL_RenderCopy(renderer, textures->get_texture(texture_id), &src, &dest);
}

const Position* Game_Object::get_obj() const {
    return &obj;
}

bool Game_Object::is_colliding(const Game_Object &obj2) {
    // We are going to get the prefered positions instead of using their object
    // For example, the block class uses tiles, so when get_x is called, it returns the x in its obj and multiplies it by the width
    Position r1, r2;
    r1.x = get_x();
    r1.y = get_y();
    r1.w = get_width();
    r1.h = get_height();

    r2.x = obj2.get_x();
    r2.y = obj2.get_y();
    r2.w = obj2.get_width();
    r2.h = obj2.get_height();


    if (r1.x < r2.x + r2.w &&
        r1.x + r1.w > r2.x &&
        r1.y < r2.y + r2.h &&
        r1.y + r1.h > r2.y) {
            return true;
        }
    return false;
}
