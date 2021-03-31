#include "scrolllist.hpp"

constexpr int SCROLLBAR_WIDTH = 20;

UI::ScrollList::ScrollList(const glm::ivec2& size,
                           const glm::ivec2& position)
    : UI::GenericComponent{COMPONENT_SCROLL_LIST, position, size},
      UI::ComponentList{},
      scrollbar{{SCROLLBAR_WIDTH, SIZE_AUTO}, {0, 0}},
      translateComponentsY{0.0}
{
    scrollbar.isScrolled = [&](const double scrollPos, const double scrollScale) {
        translateComponentsY = -(scrollPos * scrollScale);
    };
}

UI::ScrollList::~ScrollList()
{}

void UI::ScrollList::refreshContent()
{
    updateComponents();
}

void UI::ScrollList::updateComponents()
{
    int yIncrease = 0;
    
    for (auto& component: components)
    {
        std::visit([&](auto& data) {
            if (data.initialSize.x == SIZE_AUTO)
            {
                data.size.x = size.x - (scrollbar.scrollbarEnabled ? scrollbar.size.x : 0);
            }

            // Increase sizes
            data.position.x = 0;
            data.position.y = yIncrease;

            // Effects rest of components
            yIncrease += data.size.y;

            // Update component
            data.refreshContent();
        }, component);
    }

    // Update scrollbar
    const glm::ivec2& scrollbarOffset{position.x + size.x - scrollbar.size.x,
            position.y};
    scrollbar.position = scrollbarOffset;
    scrollbar.size.y = size.y;

    scrollbar.fullHeight = yIncrease;
}

void UI::ScrollList::update(const Camera& camera, const EventWrapper& events)
{
    for (auto& component: components)
    {
        std::visit([&](auto& data) {
            data.update(camera, events);
        }, component);
    }

    // Scrollbar
    scrollbar.update(camera, events);
}

void UI::ScrollList::draw(const Graphics& graphics, Transform transform) const noexcept
{
    transform.translate.x += position.x;
    transform.translate.y += translateComponentsY + position.y;

    // Clip components
    glScissor(position.x, Generic::topToBottomFlip(size.y + position.y, graphics.camera.size.h),
            size.x, size.y);

    // Components
    for (auto& component: components)
    {
        std::visit([&](auto& data) {
            data.draw(graphics, transform);
        }, component);
    }

    // Stop clipping
    glScissor(0, 0, graphics.camera.size.w, graphics.camera.size.h);

    // Scrollbar
    scrollbar.draw(graphics, transform);
}
