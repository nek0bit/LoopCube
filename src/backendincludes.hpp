#ifndef BACKENDINCLUDES_HPP
#define BACKENDINCLUDES_HPP

#include "eventwrapper_internal.hpp"
#include "graphicswrapper_internal.hpp"
#include "texture.hpp"

//********************************
//  SDL2
//********************************
#ifdef INPUT_BACKEND_SDL2
#include <SDL2/SDL.h>
#include "eventwrapper_sdl2.hpp"
#endif // INPUT_BACKEND_SDL2

#ifdef GRAPHIC_BACKEND_SDL2
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "graphicswrapper_sdl2.hpp"
#include "texture_sdl2.hpp"
#endif // GRAPHICS_BACKEND_SDL2

//********************************
//  Citro2D
//********************************
#ifdef INPUT_BACKEND_CITRO2D
//#include "eventwrapper_citro2d.hpp"
#endif

#ifdef GRAPHIC_BACKEND_CITRO2D
#include <citro2d.h>
#include "graphicswrapper_citro2d.hpp"
#include "texture_citro2d.hpp"
#endif

#endif // BACKENDINCLUDES_HPP
