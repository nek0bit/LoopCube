#include "textcomponent.hpp"

UI::TextComponent::TextComponent(const std::string& text,
                                 SDL_Color color,
                                 const glm::ivec2& size,
                                 const glm::ivec2& position,
                                 const Margin& margin)
    : GenericComponent{COMPONENT_TEXT, position, size, margin},
      textModel{UI::_ImmediateMode::_SHADER, text, color, UI::_ImmediateMode::_FONT,
    glm::vec3{0, 0, 0}, glm::vec3{1.0f, 1.0f, 1.0f}},
      textPosition{0, 0}
{
    refreshContent();
}

UI::TextComponent::~TextComponent()
{}

void UI::TextComponent::updateButtonText()
{
    textModel.createTextMesh();
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

void UI::TextComponent::update(const Camera& camera, const EventWrapper& events, Transform transform)
{
    if (initialSize.x == SIZE_AUTO) size.x = textModel.size.x;
    if (initialSize.y == SIZE_AUTO) size.y = textModel.size.y;

    int offsetX = size.x / 2 - (textModel.size.x / 2);
    int offsetY = size.y / 2 - (textModel.size.y / 2);

    // Clamp values
    if (offsetX < 0) offsetX = 0;
    if (offsetY < 0) offsetY = 0;

    textPosition = glm::ivec2(position.x + offsetX * scale.x,
                              position.y + offsetY * scale.y);
}

void UI::TextComponent::draw(const Graphics& graphics, Transform transform) const noexcept
{
    transform.translate.x += textPosition.x;
    transform.translate.y += textPosition.y;

    textModel.draw(graphics, transform);
}
