#include "menu.hpp"

Menu::Menu(const Graphics& graphics)
    : container{0, CONTAINER_HORIZONTAL, {0, 0}, {1200, 500}}
{
    // What the fuck
    container.addComponent(
        UI::Button(
            graphics.shader, 0, "Fixed: 128", constants::fontHandler[4], 128
            )
        );

    UI::Container container2{0, CONTAINER_HORIZONTAL, {0, 0}, {400, 800}};
    
    container2.addComponent(
        UI::Button(
            graphics.shader, 0, "in 1", constants::fontHandler[3]
            )
        );

    container2.addComponent(
        UI::Button(
            graphics.shader, 0, "in 2", constants::fontHandler[2]
            )
        );
    
    container2.addComponent(
        UI::Button(
            graphics.shader, 0, "in 3", constants::fontHandler[2]
            )
        );
    
    container.addComponent(
        container2);
    
    container.addComponent(
        UI::Button(
            graphics.shader, 0, "Auto", constants::fontHandler[4]
            )
        );
    
    container.addComponent(
        UI::Button(
            graphics.shader, 0, "Fixed: 256", constants::fontHandler[4], 256
            )
        );
}

Menu::~Menu()
{}

void Menu::update(const Camera& camera, const EventWrapper& events)
{
    // button.update(camera, events);
    container.update(camera, events);
}

void Menu::render(const Graphics& graphics) const noexcept
{
    // button.draw(graphics);
    container.draw(graphics);
}
