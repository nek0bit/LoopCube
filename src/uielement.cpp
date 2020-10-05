#include "uielement.hpp"

UiElement::UiElement() {}

UiElement::~UiElement() {}

void UiElement::update() {}
void UiElement::update(EventHandler& events) {}
void UiElement::update(EventHandler& events, int x_offset, int y_offset) {}
void UiElement::render() {}
void UiElement::render(SDL_Renderer* renderer) {}
void UiElement::render(SDL_Renderer* renderer, int x_offset, int y_offset) {}

