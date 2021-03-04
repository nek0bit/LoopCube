#pragma once

#include <iostream>
#include <string>

#include "../include/glad/glad.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>

struct texcoord_info
{
    unsigned width;
    unsigned height;
    unsigned tileSizeW;
    unsigned tileSizeH;
};

struct texcoord_t
{
    float begX;
    float endX;
    float begY;
    float endY;
};

struct Texture
{
    Texture(const std::string filename);
    Texture(const Texture&) = delete;
    Texture(Texture&& source);
    ~Texture();

    void bind() const;
    void freeTexture();

    static texcoord_t getTilemapCoord(const texcoord_info info, const unsigned tileX, const unsigned tileY) noexcept;
    
    GLuint texture;

    unsigned w;
    unsigned h;
};
