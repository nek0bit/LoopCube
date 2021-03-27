#include "textcomponent.hpp"

UI::TextComponent::TextComponent(const GLuint shader,
        const unsigned id,
        const std::string& text,
        SDL_Color color,
        TTF_Font* font,
        const glm::ivec2& size,
        const glm::ivec2& position)
    : GenericComponent{COMPONENT_TEXT, id, position, size},
    textModel{shader, text, color, font,
        glm::vec3{position.x, position.y, 0}, glm::vec3{size.x, size.y, 0}}
{
    textModel.scale = glm::vec3{1.0f, 1.0f, 1.0f};
    fixed |= FIXED_W | FIXED_H;
    updateButtonText();
}

UI::TextComponent::~TextComponent()
{}

void UI::TextComponent::updateButtonText()
{
    textModel.createTextMesh();
    textModel.position = glm::vec3(position.x,
            position.y,
            0.0f);
    size.x = textModel.size.x;
    size.y = textModel.size.y;
}

void UI::TextComponent::setText(const std::string& text)
{
    textModel.setText(text);
}

void UI::TextComponent::refreshContent()
{
    updateButtonText();
}

void UI::TextComponent::update(const Camera& camera, const EventWrapper& events)
{}

void UI::TextComponent::draw(const Graphics& graphics, Transform transform) const noexcept
{
    textModel.draw(graphics, transform);
}
