#include "container.hpp"

UI::Container::Container(const unsigned id,
        const glm::ivec2& position,
        const glm::ivec2& size)
    : GenericComponent{COMPONENT_CONTAINER, id, position, size},
      components{}
{}

UI::Container::~Container()
{
}

void UI::Container::updateComponents()
{
    int32_t yIncrease = position.y;
    
    // Go through each component and update its position and properties
    for (auto& component: components)
    {
        std::visit([&](auto& data) {
            // Increase size if not fixed
            if ((data.fixed & FIXED_W) == FIXED_W)
                data.size.x = size.x;
            
            if ((data.fixed & FIXED_H) == FIXED_H)
                data.size.y = size.y;
            
            // Set positions to appropriate position
            data.position.x = position.x;
            data.position.y = yIncrease;

            
            yIncrease += data.size.y;

            data.refreshContent();
        }, component.data);
    }
}

void UI::Container::update(const Camera& camera, const EventWrapper& events)
{
    for (auto& component: components)
    {
        std::visit([&](auto& data) {
            data.update(camera, events);
        }, component.data);
    }
}

void UI::Container::draw(const Graphics& graphics, const Transform& transform) const noexcept
{
    for (auto& component: components)
    {
        std::visit([&](auto& data) {
            data.draw(graphics, transform);
        }, component.data);
    }
}
