#include "text.hpp"

Text::Text(const GLuint shader,
           const std::string& text,
           const SDL_Color& color,
           TTF_Font* font,
           const glm::vec3& position,
           const glm::vec3& scale)
    : model{shader},
      surface{nullptr},
      color{color},
      font{font},
      text{text},
      texture{nullptr},
      position{position},
      scale{scale}
{    
    // Since the text has already been set
    createTextMesh();
    
    if (font == nullptr)
    {
        throw std::runtime_error("Expected TTF_Font*, got nullptr");
    }
}

Text::~Text()
{
    SDL_FreeSurface(surface);
}

void Text::createTextMesh()
{
    surface = TTF_RenderText_Blended(font, text.c_str(), color);
    texture = std::make_unique<Texture>(surface);

    // Generate and bind the data to the model
    std::vector<Vertex> data;
    if (surface) Generic::Render::generateSquare(data, 0.0f, 0.0f, surface->w, surface->h,
                                                  0.0f, 0.0f, 1.0f, 1.0f);
    model.setBufferData(data);
}

void Text::setText(const std::string& text)
{
    // Don't set and generate if the text is the exact same
    if (this->text != text)
    {
        this->text = text;
        createTextMesh();
    }
}

void Text::draw(const Graphics& graphics) const noexcept
{
    texture->bind();
    model.draw(graphics.uniforms.model, graphics.uniforms.tex, position, scale);
}
