#include "gameobj.hpp"

Game_Object::Game_Object() {}

Game_Object::Game_Object(int texture_id, TextureHandler &textures,
    double x, double y, double w, double h) {
    obj.x = x;
    obj.y = y;
    obj.w = w;
    obj.h = h;

    this->texture_id = texture_id;
    
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

double Game_Object::get_x(Camera& camera) const {
    return obj.x + (camera.get_x());
}

double Game_Object::get_y(Camera& camera) const {
    return obj.y + (camera.get_y());
}

double Game_Object::get_width() const {
    return obj.w;
}

double Game_Object::get_height() const {
    return obj.h;
}

// Used for rendering optimizations
bool Game_Object::out_of_view(Camera& camera) {
    if (get_x(camera)+(obj.w*2) < 0 || get_y(camera)+(obj.h*2) < 0) {
        return true;
    }
    if (get_x(camera)-obj.w > camera.get_width() || get_y(camera)-obj.h > camera.get_height()) {
        return true;
    }
    return false;
}

void Game_Object::update(Camera& camera) {
    src.h = get_height();
    src.w = get_width();
    src.x = 0;
    src.y = 0;

    dest.h = src.h;
    dest.w = src.w;
    dest.x = get_x(camera);
    dest.y = get_y(camera);
}

void Game_Object::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, textures->get_texture(texture_id), &src, &dest);
}

const Position& Game_Object::get_obj() const {
    return obj;
}

bool Game_Object::is_colliding(const Game_Object &obj2) {
    // We are going to get the prefered positions instead of using their object
    // For example, the block class uses tiles, so when get_x is called, it returns the x in its obj and multiplies it by the width
    Position r1{get_default_x(), get_default_y(), get_width(), get_height()},
		r2{obj2.get_default_x(), obj2.get_default_y(), obj2.get_width(), obj2.get_height()};

    if (r1.x < r2.x + r2.w &&
        r1.x + r1.w > r2.x &&
        r1.y < r2.y + r2.h &&
        r1.y + r1.h > r2.y) {
            return true;
        }
    return false;
}
