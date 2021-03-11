#include "menu.hpp"

Menu::Menu(const Graphics& graphics)
    : button{graphics.shader, 0, {50, 50}, {50, 50}}
{
    
}

Menu::~Menu()
{}

void Menu::update(const EventWrapper& events)
{
    button.update(events);
}

void Menu::render(const Graphics& graphics) const noexcept
{
    button.draw(graphics);
}
