#include "scrolllist.hpp"

constexpr int SCROLLBAR_WIDTH = 20;

UI::ScrollList::ScrollList(const unsigned id,
                           const glm::ivec2& size,
                           const glm::ivec2& position)
    : UI::GenericComponent{COMPONENT_SCROLL_LIST, id, position, size},
      UI::ComponentList{},
      scrollbar{id, -1, -1, {SCROLLBAR_WIDTH, SIZE_AUTO}, {0, 0}}
{}

UI::ScrollList::~ScrollList()
{}

void UI::ScrollList::refreshContent()
{
    updateComponents();
}

void UI::ScrollList::updateComponents()
{
    int yIncrease = position.x;
    
    for (auto& component: components)
    {
        std::visit([&](auto& data) {
            if (data.initialSize.x == SIZE_AUTO)
            {
                data.size.x = size.x - scrollbar.size.x;
            }

            // Increase sizes
            data.position.x = position.x;
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

void UI::ScrollList::draw(const Graphics& graphics, const Transform& transform) const noexcept
{
    // Components
    for (auto& component: components)
    {
        std::visit([&](auto& data) {
            data.draw(graphics, transform);
        }, component);
    }

    // Scrollbar
    scrollbar.draw(graphics, transform);
}
