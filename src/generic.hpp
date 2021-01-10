#pragma once

#include <SDL2/SDL.h>

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
}
