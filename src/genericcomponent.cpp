#include "genericcomponent.hpp"

UI::GenericComponent::GenericComponent(const unsigned id, const glm::ivec2& position, const glm::ivec2& size)
    : id{id},
      position{position},
      size{size},
      onClick{},
      scale{1.0f, 1.0f, 1.0f}
{}

UI::GenericComponent::~GenericComponent()
{}

// Calls methods 
void UI::GenericComponent::handleEvents(const Camera& camera, const EventWrapper& events)
{
    eventClick(camera, events);
}

void UI::GenericComponent::eventClick(const Camera& camera, const EventWrapper& events)
{
    if (isVmouseTouching(camera, events) && events.vmouse.clicked)
    {
        onClick();
    }
}

// Returns true if mouse is within bounds of size
bool UI::GenericComponent::isVmouseTouching(const Camera& camera, const EventWrapper& events) const noexcept
{
    return Generic::collision<float>
        (events.vmouse.x,
         Generic::topToBottomFlip<int>(events.vmouse.y, camera.size.h), 0, 0,
         position.x, position.y, size.x * scale.x, size.y * scale.y);
}

void UI::GenericComponent::update(const Camera& camera, const EventWrapper& events)
{
    handleEvents(camera, events);
}

void UI::GenericComponent::draw(const Graphics& graphics) const noexcept
{}
