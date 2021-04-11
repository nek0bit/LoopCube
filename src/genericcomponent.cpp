#include "genericcomponent.hpp"

UI::GenericComponent::GenericComponent(const component_t type,
                                       const glm::ivec2& position,
                                       const glm::ivec2& size,
                                       const Margin& margin)
    : position{position},
      realPosition{position},
      size{size},
      initialSize{size},
      type{type},
      id{0}, // 0 means not set
      onClick{[](){}},
      onHover{[](){}},
      margin{margin},
      scale{1.0f, 1.0f, 1.0f},
      fixed{0},
      parentContainer{nullptr}
{}

UI::GenericComponent::~GenericComponent()
{}

// Calls methods 
void UI::GenericComponent::handleEvents(const Camera& camera,
                                        const EventWrapper& events,
                                        const Transform& transform)
{
    eventClick(camera, events, transform);
    eventHover(camera, events, transform);
}

void UI::GenericComponent::eventClick(const Camera& camera,
                                      const EventWrapper& events,
                                      const Transform& transform)
{
    if (isVmouseTouching(camera, events, transform) && events.vmouse.clicked)
        onClick();
}

void UI::GenericComponent::eventHover(const Camera& camera,
                                      const EventWrapper& events,
                                      const Transform& transform)
{
    if (isVmouseTouching(camera, events, transform))
        onHover();
}

// Returns true if mouse is within bounds of size
bool UI::GenericComponent::isVmouseTouching(const Camera& camera,
                                            const EventWrapper& events,
                                            const Transform& transform) const noexcept
{
    // TODO isLessThanParentHeight or something like that
    // Where position.y and size.y can be affected by values being cut off by scrollList
    const int ySize = size.y * scale.y;
    const int yPos = position.y + transform.translate.y;
    const bool isMoreThanParentPos = parentContainer ? (yPos >= parentContainer->realPosition.y - ySize) : true;
    int cutOffYSize = yPos - parentContainer->realPosition.y;
    return Generic::collision<float>
        // Mouse Cursor
        (events.vmouse.x,
         events.vmouse.y, 0, 0,
         // This element
         position.x + transform.translate.x,
         yPos + std::abs(cutOffYSize),
         size.x * scale.x,
         (isMoreThanParentPos ? ySize
            : cutOffYSize + ySize));
}

void UI::GenericComponent::refreshContent()
{}

void UI::GenericComponent::update(const Camera& camera, const EventWrapper& events, Transform transform)
{
    handleEvents(camera, events, transform);
}

void UI::GenericComponent::draw(const Graphics&, Transform) const noexcept
{}
