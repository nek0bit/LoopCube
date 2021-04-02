#include "container.hpp"

UI::Container::Container(const container_layout layout,
                         const glm::ivec2& size,
                         const glm::ivec2& position,
                         const Margin& margin)
    : UI::GenericComponent{COMPONENT_CONTAINER, position, size, margin},
      UI::ComponentList{},
      layout{layout}
{}

UI::Container::~Container()
{}

// TODO Clean up this code, too much DRY
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
                fixedSize.x += data.size.x + data.margin.left + data.margin.right;
                fixedCount.x++;
            }
            
            if (data.initialSize.y != SIZE_AUTO)
            {
                fixedSize.y += data.size.y + data.margin.top + data.margin.bottom;
                fixedCount.y++;
            }
        }, component);
    }
    
    // Set it to container position at start
    float xIncrease = 0;
    float yIncrease = 0;

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
                xIncrease += sizeIncrease + data.margin.left;
            }
            else
            {
                xIncrease += data.size.x + data.margin.left;
            }
        }
        else
        {
            xIncrease += data.size.x + data.margin.left;
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
                yIncrease += sizeIncrease + data.margin.top;
            }
            else
            {
                yIncrease += data.size.y + data.margin.top;
            }
        }
        else
        {
            yIncrease += data.size.y + data.margin.top;
        }

        // Fix width of each element if possible
        if ((data.fixed & FIXED_W) != FIXED_W) data.size.x = size.x;
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
                xIncrease += data.margin.right;
                break;
            case CONTAINER_VERTICAL:
                vertical(data);
                yIncrease += data.margin.bottom;
                break;
            default: break;
            };

            data.refreshContent();
        }, component);
    }
}

void UI::Container::refreshContent()
{
    updateComponents();
}

void UI::Container::update(const Camera& camera, const EventWrapper& events)
{
    for (auto& component: components)
    {
        std::visit([&](auto& data) {
            data.update(camera, events);
        }, component);
    }
}

void UI::Container::draw(const Graphics& graphics, Transform transform) const noexcept
{
    transform.translate.x += position.x;
    transform.translate.y += position.y;

    for (auto& component: components)
    {
        std::visit([&](auto& data) {
            data.draw(graphics, transform);
        }, component);
    }
}
