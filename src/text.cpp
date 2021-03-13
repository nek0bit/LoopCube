#include "text.hpp"

Text::Text(const GLuint shader,
           const std::string& text,
           const SDL_Color& color,
           TTF_Font* font,
           const glm::vec3& position)
    : shader{shader},
      surface{nullptr},
      color{color},
      font{font},
      position{0.0f, 0.0f, 0.0f}
{
    loadTextSurface();
}

Text::~Text()
{}

void Text::loadTextSurface()
{
    surface = TTF_RenderText_Blended(font, text.c_str(), color);
}
