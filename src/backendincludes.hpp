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
//  SFML
//********************************
#ifdef GRAPHIC_BACKEND_SFML
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "graphicswrapper_sfml.hpp"
#endif // GRAPHIC_BACKEND_SFML

#endif // BACKENDINCLUDES_HPP
