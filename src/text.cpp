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
      refCount{std::make_shared<int>(1)},
      size{0, 0},
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
    if (*refCount == 1)
    {
        SDL_FreeSurface(surface);
    }

    // Bump the refCount down, another reference will probably delete it
    (*refCount)--;
}

// Copy
Text::Text(const Text& source)
    : model{source.model},
      surface{source.surface},
      color{source.color},
      font{source.font},
      text{source.text},
      texture{source.texture},
      refCount{source.refCount},
      size{source.size},
      position{source.position},
      scale{source.scale}
{
    (*refCount)++;
}

// Move
Text::Text(Text&& source)
    : model{std::move(source.model)},
      surface{std::move(source.surface)},
      color{std::move(source.color)},
      font{std::move(source.font)},
      text{std::move(source.text)},
      texture{std::move(source.texture)},
      refCount{std::move(source.refCount)},
      size{std::move(source.size)},
      position{std::move(source.position)},
      scale{std::move(source.scale)}
{
    // The same thing as above pretty much, except this is just easier
    (*refCount)++;
}

void Text::createTextMesh()
{
    using namespace std::string_literals;

    // SDL_TTF doesn't like when text is blank
    if (text == "") return;

    if ((surface = TTF_RenderText_Blended(font, text.c_str(), color)) == nullptr)
    {
        std::string msg = "Error creating text mesh: "s + TTF_GetError();
        throw std::runtime_error(msg);
    }
    
    texture = std::make_shared<Texture>(surface);

    // Generate and bind the data to the model
    std::vector<Vertex> data;
    
    if (surface)
    {
        Generic::Render::generateSquare(data, 0.0f, 0.0f, surface->w, surface->h,
                                                  0.0f, 0.0f, 1.0f, 1.0f);
        size.x = surface->w;
        size.y = surface->h;
    }
    
    model.setBufferData(data);
}

text_size Text::getTextSize(const std::string& text)
{
    using namespace std::string_literals;

    // If text is empty return default struct
    if (text == "")
        return {0, 0};
    
    SDL_Surface* textSurface;

    // It's a bit of a hack, but we will generate a rendertext surface
    if ((textSurface = TTF_RenderText_Blended(font, text.c_str(), color)) == nullptr)
    {
        std::string msg = "Error creating text mesh: "s + TTF_GetError();
        throw std::runtime_error(msg);
    }

    text_size value{textSurface->w, textSurface->h};

    // Free since it's unneeded now
    SDL_FreeSurface(textSurface);

    return value;
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

void Text::draw(const Graphics& graphics, const Transform& transform) const noexcept
{
    if (texture) {
        texture->bind();
        model.draw(graphics.uniforms.model, graphics.uniforms.tex,
                position + transform.translate,
                scale + transform.scale);
    }
}
