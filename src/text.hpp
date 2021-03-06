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
#include "transform.hpp"

struct text_size
{
    int width;
    int height;
};

class Text
{
private:
    Model model;
    SDL_Surface* surface;
    SDL_Color color;
    TTF_Font* font;
    std::string text;
    std::shared_ptr<Texture> texture;
    std::shared_ptr<int> refCount;
public:
    Text(const GLuint shader,
         const std::string& text,
         const SDL_Color& color,
         TTF_Font* font,
         const glm::vec3& position = {0.0f, 0.0f, 0.0f},
         const glm::vec3& scale = {1.0f, 1.0f, 1.0f});
    Text(const Text& source);
    Text(Text&& source);
    ~Text();

    void createTextMesh();
    void setText(const std::string& text);
    void draw(const Graphics& graphics, const Transform& transform = {}) const noexcept;
    inline const std::string& getText() const noexcept { return text; }

    text_size getTextSize(const std::string& text) const;

    glm::ivec2 size;
    glm::vec3 position;
    glm::vec3 scale;
};
