#pragma once

#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

struct Texture
{
    Texture(SDL_Renderer* renderer, std::string filename);
    ~Texture();

    SDL_Texture*& getTexture();
    void freeTexture();
private:
    SDL_Texture* texture;
};
