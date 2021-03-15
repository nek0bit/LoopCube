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
