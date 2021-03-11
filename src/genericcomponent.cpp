#include "genericcomponent.hpp"

UI::GenericComponent::GenericComponent(const unsigned id, const glm::ivec2& position, const glm::ivec2& size)
    : id{id},
      position{position},
      size{size},
      clickFunctor{std::nullopt},
      enterFunctor{std::nullopt},
      exitFunctor{std::nullopt}
{}

UI::GenericComponent::~GenericComponent()
{}

void UI::GenericComponent::onClick(const std::function<void(void)>& clickFunctor) noexcept
{
    this->clickFunctor = clickFunctor;
}

void UI::GenericComponent::onEnter(const std::function<void(void)>& enterFunctor) noexcept
{
    this->enterFunctor = enterFunctor;
}

void UI::GenericComponent::onExit(const std::function<void(void)>& exitFunctor) noexcept
{
    this->exitFunctor = exitFunctor;
}

void UI::GenericComponent::update(const EventWrapper& events)
{}

void UI::GenericComponent::draw(const Graphics& graphics) const noexcept
{}
