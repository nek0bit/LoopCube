#include "genericcomponent.hpp"

UI::GenericComponent::GenericComponent(const unsigned id, const glm::ivec2& position, const glm::ivec2& size)
    : id{id},
      position{position},
      size{size},
      clickFunctor{std::nullopt}
      /*
      enterFunctor{std::nullopt},
      exitFunctor{std::nullopt}
      */
{}

UI::GenericComponent::~GenericComponent()
{}

void UI::GenericComponent::onClick(const std::function<void(unsigned)>& clickFunctor) noexcept
{
    this->clickFunctor = clickFunctor;
}

/*
void UI::GenericComponent::onEnter(const std::function<void(void)>& enterFunctor) noexcept
{
    this->enterFunctor = enterFunctor;
}

void UI::GenericComponent::onExit(const std::function<void(void)>& exitFunctor) noexcept
{
    this->exitFunctor = exitFunctor;
}
*/

// Calls methods 
void UI::GenericComponent::handleEvents(const Camera& camera, const EventWrapper& events)
{
    eventClick(camera, events);
    // Possible methods to implement
    //eventEnter(events);
    //eventExit(events);
}

void UI::GenericComponent::eventClick(const Camera& camera, const EventWrapper& events)
{
    if (isVmouseTouching(camera, events) && events.vmouse.clicked)
    {
        if (clickFunctor != std::nullopt) (*clickFunctor)(id);
    }
}

// Returns true if mouse is within bounds of size
bool UI::GenericComponent::isVmouseTouching(const Camera& camera, const EventWrapper& events) const noexcept
{
    return Generic::collision<float>(events.vmouse.x,
                                     Generic::topToBottomFlip<int>(events.vmouse.y, camera.size.h),
                                     0, 0, position.x, position.y, size.x, size.y);
}

void UI::GenericComponent::update(const Camera& camera, const EventWrapper& events)
{
    handleEvents(camera, events);
}

void UI::GenericComponent::draw(const Graphics& graphics) const noexcept
{}
