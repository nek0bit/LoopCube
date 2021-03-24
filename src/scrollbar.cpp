#include "scrollbar.hpp"

UI::Scrollbar::Scrollbar(const unsigned id,
                         int fullHeight,
                         int viewHeight,
                         const glm::ivec2& size,
                         const glm::ivec2& position)
    : GenericComponent{COMPONENT_SCROLLBAR, id, position, size},
    scrollPosition{0.0},
    fullHeight{fullHeight},
    viewHeight{viewHeight},
    scrollbarEnabled{size.y < fullHeight},
    isScrolled{[](double d){}}
{}

UI::Scrollbar::~Scrollbar() {}

void UI::Scrollbar::update(const Camera& camera, const EventWrapper& events)
{
    // Calculate height
    scrollbarEnabled = size.y < fullHeight;
    viewHeight = size.y / static_cast<float>(fullHeight);
    if (isClickingScrollbar(events))
    {
        isScrolled(scrollPosition);
    }
}

bool UI::Scrollbar::isClickingScrollbar(const EventWrapper& events) const noexcept
{
    return events.vmouse.down &&
        Generic::collision<float>(events.vmouse.x, events.vmouse.y, 0, 0,
                position.x, position.y, size.x, viewHeight * size.y);
}

void UI::Scrollbar::draw(const Graphics& graphics, const Transform& transform) const noexcept
{
    if (scrollbarEnabled)
    {
        constexpr int Z_BUF = 0;
        graphics.textures.getTexture(TEXTURE_SCROLLBAR_BG)->bind();
        graphics.models.getModel(MODEL_SQUARE).draw(graphics.uniforms.model,
                graphics.uniforms.tex,
                glm::vec3{position.x, position.y, Z_BUF},
                glm::vec3{size.x, size.y, Z_BUF});

        // Draw scrollView
        graphics.textures.getTexture(TEXTURE_SCROLLBAR)->bind();
        graphics.models.getModel(MODEL_SQUARE).draw(graphics.uniforms.model,
                graphics.uniforms.tex,
                glm::vec3{position.x, position.y, Z_BUF},
                glm::vec3{size.x, viewHeight * size.y, Z_BUF});
    }
}
