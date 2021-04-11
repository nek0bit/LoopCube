#include "scrolllist.hpp"

constexpr int SCROLLBAR_WIDTH = 20;

UI::ScrollList::ScrollList(const glm::ivec2& size,
                           const glm::ivec2& position,
                           const Margin& margin)
    : UI::GenericComponent{COMPONENT_SCROLL_LIST, position, size, margin},
      UI::ComponentList{},
      scrollbar{{SCROLLBAR_WIDTH, SIZE_AUTO}, {0, 0}},
      translateComponentsY{0.0}
{}

UI::ScrollList::~ScrollList()
{}

void UI::ScrollList::refreshContent()
{
    updateComponents();

    // Regenerate scroll call
    scrollbar.isScrolled = [&](const double scrollPos, const double scrollScale) {
        translateComponentsY = -(scrollPos * scrollScale);
    };
}

void UI::ScrollList::updateComponents()
{
    int yIncrease = 0;
    
    for (auto& component: components)
    {
        std::visit([&](auto& data) {
            data.parentContainer = this;
            if (data.initialSize.x == SIZE_AUTO)
            {
                data.size.x = size.x - (scrollbar.scrollbarEnabled ? scrollbar.size.x : 0)
                    - data.margin.left - data.margin.right;
            }

            yIncrease += data.margin.top;

            // Increase sizes
            data.position.x = data.margin.left;
            data.position.y = yIncrease;

            // Effects rest of components
            yIncrease += data.size.y + data.margin.bottom;

            // Update component
            data.refreshContent();
        }, component);
    }

    // Update scrollbar
    const glm::ivec2& scrollbarOffset{size.x - scrollbar.size.x,
            0};
    scrollbar.position = scrollbarOffset;
    scrollbar.size.y = size.y;

    scrollbar.fullHeight = yIncrease;
}

void UI::ScrollList::update(const Camera& camera, const EventWrapper& events, Transform transform)
{
    transform.translate.x += position.x;
    transform.translate.y += position.y;

    realPosition = transform.translate;

    // Scrollbar
    scrollbar.update(camera, events, transform);

    // Increase transform again for components
    transform.translate.y += translateComponentsY;
    for (auto& component: components)
    {
        std::visit([&](auto& data) {
            data.update(camera, events, transform);
        }, component);
    }

}

void UI::ScrollList::draw(const Graphics& graphics, Transform transform) const noexcept
{
    transform.translate.x += position.x;
    transform.translate.y += position.y;

    // Clip components
    glScissor(transform.translate.x, Generic::topToBottomFlip<double>(size.y + transform.translate.y, graphics.camera.size.h),
            size.x, size.y);

    // Increase transform for components
    transform.translate.y += translateComponentsY;
    // Components
    for (auto& component: components)
    {
        std::visit([&](auto& data) {
            data.draw(graphics, transform);
        }, component);
    }

    // Stop clipping
    glScissor(0, 0, graphics.camera.size.w, graphics.camera.size.h);

    // Decrease transform back
    transform.translate.y -= translateComponentsY;

    // Scrollbar
    scrollbar.draw(graphics, transform);
}
