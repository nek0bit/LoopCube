#pragma once

#include <iostream>

#include <SDL2/SDL.h>

#include "camera.hpp"
#include "time.hpp"

struct Background
{
    Background();
    virtual ~Background();

    virtual void update(Camera&, Time&);
    virtual void render(SDL_Renderer*, TextureHandler&);
private:
    
};
