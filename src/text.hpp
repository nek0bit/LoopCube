#pragma once
#include <string>
#include <memory>
#include <exception>
#include <stdexcept>

#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../include/glad/glad.h"
#include "generic.hpp"
#include "graphics.hpp"
#include "model.hpp"
#include "texture.hpp"

class Text
{
private:
    Model model;
    SDL_Surface* surface;
    SDL_Color color;
    TTF_Font* font;
    std::string text;
    std::unique_ptr<Texture> texture;
public:
    Text(const GLuint shader,
         const std::string& text,
         const SDL_Color& color,
         TTF_Font* font,
         const glm::vec3& position = {0.0f, 0.0f, 0.0f},
         const glm::vec3& scale = {0.0f, 0.0f, 0.0f});
    ~Text();

    void createTextMesh();
    void setText(const std::string& text);
    void draw(const Graphics& graphics) const noexcept;
    inline const std::string& getText() const noexcept { return text; };

    glm::ivec2 size;
    glm::vec3 position;
    glm::vec3 scale;
};
