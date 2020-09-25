#ifndef GRAVITYPARTICLE_HPP
#define GRAVITYPARTICLE_HPP
#include <iostream>
#include "gameobj.hpp"

class GravityParticle: public Game_Object {
public:
    GravityParticle() = default;
    GravityParticle(int texture_id, TextureHandler& textures, int time,
        int start_vel_x, int start_vel_y, int x, int y, int width=10, int height=10);
    ~GravityParticle();
    
    void update(Camera& camera);
    
    // Returns true if time reached
    // Appropriate to destruct or quit rendering when done.
    bool is_dead() const;
private:
    double vel_x;
    double vel_y;
    int time;
    int time_total;
};


#endif // GRAVITYPARTICLE_HPP