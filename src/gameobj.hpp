#ifndef GAMEOBJ_HPP
#define GAMEOBJ_HPP
#include "SDL2/SDL.h"
#include <string>

#include "texturehandler.hpp"
#include "constants.hpp"
#include "position.hpp"
#include "camera.hpp"

class Game_Object {

public:
    Game_Object();
    Game_Object(int texture_id, TextureHandler &textures, double x, double y, double w, double h);
    virtual ~Game_Object();

    virtual void update(Camera& camera);
    virtual void render(SDL_Renderer* renderer);

    virtual bool out_of_view(Camera& camera);

    // Return position
    virtual double get_default_x() const;
    virtual double get_default_y() const;
    virtual double get_x(Camera& camera) const;
    virtual double get_y(Camera& camera) const;
    virtual double get_width() const;
    virtual double get_height() const;

    virtual const Position* get_obj() const;

    bool is_colliding(const Game_Object &obj2, Camera& camera);
protected:
    int texture_id;

    Position obj;
    SDL_Rect src, dest;
    TextureHandler* textures;

};


#endif // GAMEOBJ_HPP
