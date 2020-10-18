#include "uielement.hpp"

UiElement::UiElement() {}

UiElement::~UiElement() {}

void UiElement::on_change(void (*)(int, int)) {}
int UiElement::get_id() {return 0;}
bool UiElement::is_changed() {return false;}
void UiElement::get_value(bool&) {}
void UiElement::set_x(int) {}
void UiElement::set_y(int) {}
void UiElement::update() {}
void UiElement::update(EventHandler&) {}
void UiElement::update(EventHandler&, int, int) {}
void UiElement::render() {}
void UiElement::render(SDL_Renderer*) {}
void UiElement::render(SDL_Renderer*, int, int) {}

