#include "generic.hpp"

bool Generic::collision(SDL_Rect r1, SDL_Rect r2)
{
    return Generic::collision(r1.x, r1.y, r1.w, r1.h, r2.x, r2.y, r2.w, r2.h);
}
