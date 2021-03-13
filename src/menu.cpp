#include "menu.hpp"

Menu::Menu(const Graphics& graphics)
    : button{graphics.shader, 0, "Test", constants::fontHandler.getFont(0), {50, 50}, 128}
{
    button.onClick = [&button = button]() {
        button.scale += glm::vec3(1.0f, 1.0f, 0.0f);
    };
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
