#include "genericcomponent.hpp"

UI::GenericComponent::GenericComponent(const component_t type,
                                       const glm::ivec2& position,
                                       const glm::ivec2& size,
                                       const Margin& margin)
    : position{position},
      size{size},
      initialSize{size},
      type{type},
      id{0}, // 0 means not set
      onClick{[](){}},
      onHover{[](){}},
      margin{margin},
      scale{1.0f, 1.0f, 1.0f},
      fixed{0}
{}

UI::GenericComponent::~GenericComponent()
{}

// Calls methods 
void UI::GenericComponent::handleEvents(const Camera& camera, const EventWrapper& events, const Transform& transform)
{
    eventClick(camera, events, transform);
    eventHover(camera, events, transform);
}

void UI::GenericComponent::eventClick(const Camera& camera, const EventWrapper& events, const Transform& transform)
{
    if (isVmouseTouching(camera, events, transform) && events.vmouse.clicked)
    {
        onClick();
    }
}

void UI::GenericComponent::eventHover(const Camera& camera, const EventWrapper& events, const Transform& transform)
{
    if (isVmouseTouching(camera, events, transform))
    {
        onHover();
    }
}

// Returns true if mouse is within bounds of size
bool UI::GenericComponent::isVmouseTouching(const Camera& camera, const EventWrapper& events, const Transform& transform) const noexcept
{
    return Generic::collision<float>
        (events.vmouse.x,
         events.vmouse.y, 0, 0,
         position.x + transform.translate.x, position.y + transform.translate.y, size.x * scale.x, size.y * scale.y);
}

void UI::GenericComponent::refreshContent()
{}

void UI::GenericComponent::update(const Camera& camera, const EventWrapper& events, Transform transform)
{
    handleEvents(camera, events, transform);
}

void UI::GenericComponent::draw(const Graphics&, Transform) const noexcept
{}
