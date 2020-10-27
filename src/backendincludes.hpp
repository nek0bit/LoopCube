#ifndef BACKENDINCLUDES_HPP
#define BACKENDINCLUDES_HPP

#include "eventwrapper_internal.hpp"

#ifdef INPUT_BACKEND_SDL2
#include <SDL2/SDL.h>
#include "eventwrapper_sdl2.hpp"
#endif // INPUT_BACKEND_SDL2

#ifdef GRAPHIC_BACKEND_SDL2
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#endif // GRAPHICS_BACKEND_SDL2

#endif // BACKENDINCLUDES_HPP
