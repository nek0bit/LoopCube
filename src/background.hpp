#pragma once

#include <iostream>

#include "graphics.hpp"
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

    virtual void update(const Camera&, Time&);
    virtual void render(const Graphics&, TextureHandler&) const;
};
