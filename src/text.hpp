#pragma once
#include <string>
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../include/glad/glad.h"

class Text
{
private:
    const GLuint shader;
    SDL_Surface* surface;
    SDL_Color color;
    TTF_Font* font;
    std::string text;
public:
    Text(const GLuint shader,
         const std::string& text,
         const SDL_Color& color,
         TTF_Font* font,
         const glm::vec3& position = {0, 0, 0});
    ~Text();

    void loadTextSurface();

    glm::vec3 position;
};
