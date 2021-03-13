#include "menu.hpp"

Menu::Menu(const Graphics& graphics)
    : button{graphics.shader, 0, {50, 50}, 128}
{
    button.onClick([](unsigned id) {
        std::cout << "Button " << id << " clicked" << std::endl;
        });
}

Menu::~Menu()
{}

void Menu::update(const Camera& camera, const EventWrapper& events)
{
    button.update(camera, events);
}

void Menu::render(const Graphics& graphics) const noexcept
{
    button.draw(graphics);
}
