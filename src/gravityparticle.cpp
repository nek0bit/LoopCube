#include "gravityparticle.hpp"

GravityParticle::GravityParticle(int texture_id, TextureHandler& textures, int time, int start_vel_x,
    int start_vel_y, int x, int y, int width, int height) : Game_Object{texture_id,
        textures, x, y, width, height}, vel_x{start_vel_x}, vel_y{start_vel_y}, time_total{time}, time{0} {
    
}

GravityParticle::~GravityParticle() {}

// TODO move engine code (as previous comment said) for reusability.
void GravityParticle::update(Camera& camera) {
    time += 1;
    
    vel_x *= 0.95;
    obj.x += vel_x;
    
    vel_y += .5;
    obj.y += vel_y;
    
    // Update draw position
    src.h = get_height();
    src.w = get_width();
    src.x = 0;
    src.y = 0;
    
    dest.h = src.h;
    dest.w = src.w;
    dest.x = get_x(camera);
    dest.y = get_y(camera);
}

bool GravityParticle::is_dead() const {
    return time > time_total; 
}