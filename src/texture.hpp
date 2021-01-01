#pragma once

#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Texture
{
    Texture(SDL_Renderer* renderer, std::string filename);
    ~Texture();

    SDL_Texture*& get_texture();
    void free_texture();
private:
    SDL_Texture* texture;
};
