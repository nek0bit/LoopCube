#pragma once

#include <SDL2/SDL.h>

#include "texturehandler.hpp"

struct GridCollision_t
{
    unsigned int width;
    unsigned int height;
};

namespace Generic
{
    template <typename CMP>
    bool collision(CMP x1, CMP y1, CMP w1, CMP h1,
                   CMP x2, CMP y2, CMP w2, CMP h2)
    {
        return (x1 < x2 + w2 &&
                x1 + w1 > x2 &&
                y1 < y2 + h2 &&
                y1 + h1 > y2);
    }

    bool collision(SDL_Rect r1, SDL_Rect r2);

    GridCollision_t gridCollision(unsigned int width, unsigned int height, SDL_Rect box);

    namespace Render
    {
        void renderRepeating(SDL_Renderer* renderer, TextureHandler& textures,
                             int texture, int clipWidth,
                             int clipHeight, int offsetX, int offsetY,
                             int width, int height, int gap, int top,
                             bool verticle = false, int srcW = -1, int srcH = -1);
    }
}
