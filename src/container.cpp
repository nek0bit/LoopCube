#include "container.hpp"

UI::Container::Container(const unsigned id,
                         const container_layout layout,
                         const glm::ivec2& position,
                         const glm::ivec2& size)
    : UI::GenericComponent{COMPONENT_CONTAINER, id, position, size},
      UI::ComponentList{},
      layout{layout}
{}

UI::Container::~Container()
{}

void UI::Container::updateComponents()
{
    // Get total size of all fixed size components
    glm::ivec2 fixedSize{0, 0};
    glm::ivec2 fixedCount{0, 0};
    for (auto& component: components)
    {
        std::visit([&](auto& data) {
            if (data.initialSize.x != SIZE_AUTO)
            {
                fixedSize.x += data.size.x;
                fixedCount.x++;
            }
            
            if (data.initialSize.y != SIZE_AUTO)
            {
                fixedSize.y += data.size.y;
                fixedCount.y++;
            }
        }, component.data);
    }
    
    // Set it to container position at start
    float xIncrease = position.x;
    float yIncrease = position.y;

    // For each layout...
    // Horizontal layout
    auto horizontal = [&](auto& data) {
        const float sizeIncrease = static_cast<float>(size.x - fixedSize.x)
            / static_cast<float>(components.size() - fixedCount.x);

        // Increase size if not fixed
        if ((data.fixed & FIXED_W) != FIXED_W)
        {
            if (data.initialSize.x == SIZE_AUTO)
            {
                data.size.x = sizeIncrease;
                xIncrease += sizeIncrease;
            }
            else
            {
                xIncrease += data.size.x;
            }
        }

        // Fix height of each element if possible
        if ((data.fixed & FIXED_H) != FIXED_H) data.size.y = size.y;        
    };

    // Vertical layout
    auto vertical = [&](auto& data) {
        const float sizeIncrease = static_cast<float>(size.y - fixedSize.y)
            / static_cast<float>(components.size() - fixedCount.y);

        // Increase size if not fixed
        if ((data.fixed & FIXED_H) != FIXED_H)
        {
            if (data.initialSize.y == SIZE_AUTO)
            {
                data.size.y = sizeIncrease;
                yIncrease += sizeIncrease;
            }
            else
            {
                yIncrease += data.size.y;
            }
        }

        // Fix width of each element if possible
        if ((data.fixed & FIXED_W) != FIXED_W) data.size.x = size.x;
    };
    
    // Go through each component and update its position and properties
    for (size_t i = 0; i < components.size(); ++i)
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
        }, components[i].data);
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
