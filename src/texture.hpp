#pragma once

#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

struct Texture
{
    Texture(SDL_Renderer* renderer, std::string filename, unsigned int w, unsigned int h);
    ~Texture();

    void freeTexture();
    
    SDL_Texture* texture;

    unsigned int w;
    unsigned int h;
};
