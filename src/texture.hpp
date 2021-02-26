#pragma once

#include <iostream>
#include <string>

#include "../include/glad/glad.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>

struct Texture
{
    Texture(const std::string filename);
    ~Texture();

    void bind() const;
    void freeTexture();
    
    GLuint texture;

    unsigned w;
    unsigned h;
};
