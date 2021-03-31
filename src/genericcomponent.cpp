#include "genericcomponent.hpp"

/* Some TODOs
 * - Make constructing components easier and cleaner (without constructors
 *   being full of symbols)
 * - margins
 */

UI::GenericComponent::GenericComponent(const component_t type,
                                       const glm::ivec2& position,
                                       const glm::ivec2& size)
    : position{position},
      size{size},
      initialSize{size},
      type{type},
      id{0}, // 0 means not set
      onClick{[](){}},
      onHover{[](){}},
      scale{1.0f, 1.0f, 1.0f},
      fixed{0}
{}

UI::GenericComponent::~GenericComponent()
{}

// Calls methods 
void UI::GenericComponent::handleEvents(const Camera& camera, const EventWrapper& events)
{
    eventClick(camera, events);
    eventHover(camera, events);
}

void UI::GenericComponent::eventClick(const Camera& camera, const EventWrapper& events)
{
    if (isVmouseTouching(camera, events) && events.vmouse.clicked)
    {
        onClick();
    }
}

void UI::GenericComponent::eventHover(const Camera& camera, const EventWrapper& events)
{
    if (isVmouseTouching(camera, events))
    {
        onHover();
    }
}

// Returns true if mouse is within bounds of size
bool UI::GenericComponent::isVmouseTouching(const Camera& camera, const EventWrapper& events) const noexcept
{
    return Generic::collision<float>
        (events.vmouse.x,
         events.vmouse.y, 0, 0,
         position.x, position.y, size.x * scale.x, size.y * scale.y);
}

void UI::GenericComponent::refreshContent()
{}

void UI::GenericComponent::update(const Camera& camera, const EventWrapper& events)
{
    handleEvents(camera, events);
}

void UI::GenericComponent::draw(const Graphics&, Transform) const noexcept
{}
