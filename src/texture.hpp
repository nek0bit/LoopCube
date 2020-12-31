#pragma once

#include <string>

class Texture
{
    Texture(SDL_Renderer* renderer, std::string filename);
    ~Texture();

    SDL_Texture*& get_texture();
    void free_texture();
private:
    SDL_Texture* texture;
};
