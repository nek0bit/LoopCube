#include "uielement.hpp"

UiElement::UiElement() {}

UiElement::~UiElement() {}

void UiElement::onChange(void (*)(int, int)) {}
bool UiElement::isChanged() {return false;}
void UiElement::getValue(bool&) {}
void UiElement::setX(int) {}
void UiElement::setY(int) {}
void UiElement::update() {}
void UiElement::update(EventWrapper&, Timer&) {}
void UiElement::update(EventWrapper&, Timer&, int, int) {}
void UiElement::render() {}
void UiElement::render(SDL_Renderer*, TextureHandler&) {}
void UiElement::render(SDL_Renderer*, TextureHandler&, int, int) {}

