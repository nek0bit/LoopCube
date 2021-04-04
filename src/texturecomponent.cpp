#include "texturecomponent.hpp"

UI::TextureComponent::TextureComponent(const size_t textureId,
                                       const glm::ivec2& size,
                                       const glm::ivec2& position,
                                       const Margin& margin)
    : GenericComponent{COMPONENT_TEXTURE, position, size, margin},
      textureId{textureId}
{
    refreshContent();
}

UI::TextureComponent::~TextureComponent()
{}

void UI::TextureComponent::refreshContent()
{}

void UI::TextureComponent::update(const Camera& camera, const EventWrapper& events, Transform transform)
{}

void UI::TextureComponent::draw(const Graphics& graphics, Transform transform) const noexcept
{
    graphics.textures.getTexture(textureId)->bind();
    graphics.models.getModel(MODEL_SQUARE).draw(graphics.uniforms.model, graphics.uniforms.tex,
                                                glm::vec3(position.x, position.y, 0.0f) + transform.translate,
                                                scale + transform.scale);
}
