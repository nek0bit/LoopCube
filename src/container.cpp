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

void UI::Container::update(const Camera& camera, const EventWrapper& events)
{
    for (auto& component: components)
    {    
        switch (component.type)
        {
        case COMPONENT_BUTTON:
            std::get<Button>(component.data).update(camera, events);
        default:
            break;
        }
    }
}

void UI::Container::draw(const Graphics& graphics, const Transform& transform) const noexcept
{
    // TODO use std::visit instead of this crap
    for (auto& component: components)
    {
        switch (component.type)
        {
        case COMPONENT_BUTTON:
            std::get<Button>(component.data).draw(graphics, transform);
        default:
            break;
        }
    }
}
