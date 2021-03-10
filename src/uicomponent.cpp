#include "uicomponent.hpp"

UiComponent::UiComponent(const int id, const glm::ivec2& position, const glm::ivec2& size)
    : id{id},
      position{0, 0},
      size{0, 0},
      clickFunc{std::nullopt},
      enterFunc{std::nullopt},
      exitFunc{std::nullopt}
{}

UiComponent::~UiComponent()
{}

void onClick(const std::function<void(void)>& clickFunctor)
{}

void onEnter(const std::function<void(void)>& enterFunctor)
{}


void onExit(const std::function<void(void)>& exitFunctor)
{}
