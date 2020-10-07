#include "uielement.hpp"

UiElement::UiElement() {}

UiElement::~UiElement() {}

void UiElement::on_change(void (*function)(int, int)) {};
int UiElement::get_id() {}
bool UiElement::is_changed() {}
void UiElement::get_value(bool&) {}
void UiElement::set_x(int x) {}
void UiElement::set_y(int y) {}
void UiElement::update() {}
void UiElement::update(EventHandler& events) {}
void UiElement::update(EventHandler& events, int x_offset, int y_offset) {}
void UiElement::render() {}
void UiElement::render(SDL_Renderer* renderer) {}
void UiElement::render(SDL_Renderer* renderer, int x_offset, int y_offset) {}

