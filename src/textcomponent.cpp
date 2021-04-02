#include "textcomponent.hpp"

UI::TextComponent::TextComponent(const std::string& text,
                                 SDL_Color color,
                                 const glm::ivec2& size,
                                 const glm::ivec2& position,
                                 const Margin& margin)
    : GenericComponent{COMPONENT_TEXT, position, size, margin},
      textModel{UI::_ImmediateMode::_SHADER, text, color, UI::_ImmediateMode::_FONT,
    glm::vec3{position.x, position.y, 0}, glm::vec3{size.x, size.y, 0}}
{
    textModel.scale = glm::vec3{1.0f, 1.0f, 1.0f};
    refreshContent();
}

UI::TextComponent::~TextComponent()
{}

void UI::TextComponent::updateButtonText()
{
    const uint16_t offsetX = size.x / 2 - (textModel.size.x / 2);
    const uint16_t offsetY = size.y / 2 - (textModel.size.y / 2);
    textModel.createTextMesh();
    textModel.position = glm::vec3(position.x + offsetX * scale.x,
            position.y + offsetY * scale.y,
            0.0f);
}

void UI::TextComponent::setText(const std::string& text)
{
    textModel.setText(text);
    refreshContent();
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
