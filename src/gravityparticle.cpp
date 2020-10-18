#include "gravityparticle.hpp"

GravityParticle::GravityParticle(int texture_id, TextureHandler& textures, int time, int start_vel_x,
								 int start_vel_y, int x, int y, int width, int height) : Game_Object{texture_id,
	textures, static_cast<double>(x), static_cast<double>(y), static_cast<double>(width), static_cast<double>(height)},
	vel_x{static_cast<double>(start_vel_x)}, vel_y{static_cast<double>(start_vel_y)}, time{0}, time_total{time} {}

GravityParticle::~GravityParticle() {}

bool GravityParticle::check_block_collision(Chunk_Group& chunks) { // IGNORE THIS WARNING: I'm going to rewrite GravityParticle to use Entity class soon.
	(void)chunks;
    return false;
}



// TODO move engine code (as previous comment said) for reusability.
void GravityParticle::update(Chunk_Group &chunks, Camera& camera) { // IGNORE THIS WARNING: I'm going to rewrite GravityParticle to use Entity class soon.
	(void)chunks;
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
