#pragma once

#include <iostream>

#include <SDL2/SDL.h>

#include "texturehandler.hpp"
#include "camera.hpp"
#include "time.hpp"

struct BGCoord
{
    int x;
    int y;
};

struct BGSize
{
    int w;
    int h;
};

struct Background
{
    Background();
    virtual ~Background();

    virtual void update(Camera&, Time&);
    virtual void render(SDL_Renderer*, TextureHandler&);    
};
