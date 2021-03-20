#include "scrollbar.hpp"

UI::Scrollbar::Scrollbar(const unsigned id,
                         int fullHeight,
                         int viewHeight,
                         const glm::ivec2& size,
                         const glm::ivec2& position)
    : GenericComponent{COMPONENT_SCROLLBAR, id, position, size},
      fullHeight{fullHeight},
      viewHeight{viewHeight}
{}

UI::Scrollbar::~Scrollbar() {}

void UI::Scrollbar::update(const Camera& camera, const EventWrapper& events)
{
    
}

void UI::Scrollbar::draw(const Graphics& graphics, const Transform& transform) const noexcept
{
    constexpr int Z_BUF = 0;
    graphics.textures.getTexture(TEXTURE_PLAYER)->bind();
    graphics.models.getModel(MODEL_SQUARE).draw(graphics.uniforms.model,
                                                graphics.uniforms.tex,
                                                glm::vec3{position.x, position.y, Z_BUF},
                                                glm::vec3{size.x, size.y, Z_BUF});
}
