#include "scrolllist.hpp"

UI::ScrollList::ScrollList(const unsigned id,
                           const glm::ivec2& size,
                           const glm::ivec2& position)
    : UI::GenericComponent{COMPONENT_SCROLL_LIST, id, position, size},
      UI::ComponentList{}
{}

UI::ScrollList::~ScrollList()
{}

void UI::ScrollList::updateComponents()
{
    int yIncrease = position.x;
    
    for (auto& component: components)
    {
        std::visit([&](auto& data) {
            data.position.x = position.x;
            data.position.y = yIncrease;
            yIncrease += data.size.y;
        }, component);
    }
}

void UI::ScrollList::update(const Camera& camera, const EventWrapper& events)
{
    for (auto& component: components)
    {
        std::visit([&](auto& data) {
            data.update(camera, events);
        }, component);
    }
}

void UI::ScrollList::draw(const Graphics& graphics, const Transform& transform) const noexcept
{
    
    for (auto& component: components)
    {
        std::visit([&](auto& data) {
            data.draw(graphics, transform);
        }, component);
    }
}
