#include "scrollbar.hpp"

UI::Scrollbar::Scrollbar(const unsigned id,
                         int fullHeight,
                         int viewHeight,
                         const glm::ivec2& size,
                         const glm::ivec2& position)
    : GenericComponent{COMPONENT_SCROLLBAR, id, position, size},
    scrollPosition{0.0},
    scrollScale{0.0},
    fullHeight{fullHeight},
    viewHeight{viewHeight},
    scrollbarEnabled{size.y < fullHeight},
    isBeingDragged{false},
    isScrolled{[](double, double){}}
{}

UI::Scrollbar::~Scrollbar() {}

void UI::Scrollbar::update(const Camera& camera, const EventWrapper& events)
{
    // Calculate height
    scrollbarEnabled = size.y < fullHeight;
    viewHeight = size.y / static_cast<float>(fullHeight);
    
    static int lastMousePos = events.vmouse.y;

    // Set to dragging state
    if (isClickingScrollbar(events))
        isBeingDragged = true;

    if (isBeingDragged)
    {
        scrollScale = static_cast<double>(size.y) / (viewHeight * size.y);
        scrollPosition += events.vmouse.y - lastMousePos; // Real scroll position
        // Fix scrollbar from going in negatives and similar things
        fixScrollbar();
        isScrolled(scrollPosition, scrollScale);
    }

    // Stop dragging when mouse down is released
    if (isBeingDragged && events.vmouse.down != 1)
        isBeingDragged = false;

    lastMousePos = events.vmouse.y;
}

void UI::Scrollbar::fixScrollbar() noexcept
{
    const int maxView = size.y - (viewHeight * size.y);

    if (scrollPosition < 0)
        scrollPosition = 0;
    else if (scrollPosition > maxView)
        scrollPosition = maxView;

}

bool UI::Scrollbar::isClickingScrollbar(const EventWrapper& events) const noexcept
{
    return events.vmouse.clicked == 1 &&
        Generic::collision<float>(events.vmouse.x, events.vmouse.y, 0, 0,
                position.x, position.y + scrollPosition, size.x, viewHeight * size.y);
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
                glm::vec3{position.x, position.y + scrollPosition, Z_BUF},
                glm::vec3{size.x, viewHeight * size.y, Z_BUF});
    }
}
