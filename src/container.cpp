#include "container.hpp"

UI::Container::Container(const unsigned id,
                         const container_layout layout,
                         const glm::ivec2& position,
                         const glm::ivec2& size)
    : GenericComponent{COMPONENT_CONTAINER, id, position, size},
      components{},
      layout{layout}
{}

UI::Container::~Container()
{}

void UI::Container::updateComponents()
{
    float xIncrease = position.x;
    float yIncrease = position.y;

    // For each layout
    // Horizontal layout
    auto horizontal = [&](auto& data) {
        const float sizeIncrease = size.x / components.size();

        // Increase size if not fixed
        if ((data.fixed & FIXED_W) != FIXED_W)
            data.size.x = sizeIncrease;

        if ((data.fixed & FIXED_H) != FIXED_H)
            data.size.y = size.y;
            
        xIncrease += sizeIncrease;
    };

    // Vertical layout
    auto vertical = [&](auto& data) {
        const float sizeIncrease = size.y / components.size();

        // Increase size if not fixed
        if ((data.fixed & FIXED_W) != FIXED_W)
            data.size.x = size.x;

        if ((data.fixed & FIXED_H) != FIXED_H)
            data.size.y = sizeIncrease;
            
        yIncrease += sizeIncrease;    
    };
    
    // Go through each component and update its position and properties
    for (auto& component: components)
    {
        std::visit([&](auto& data) {
            data.position.x = xIncrease;
            data.position.y = yIncrease;
            
            switch(layout)
            {
            case CONTAINER_HORIZONTAL:
                horizontal(data);
                break;
            case CONTAINER_VERTICAL:
                vertical(data);
                break;
            default: break;
            };

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
