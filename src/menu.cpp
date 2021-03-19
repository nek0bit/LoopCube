#include "menu.hpp"

Menu::Menu(const Graphics& graphics)
    : container{0, CONTAINER_HORIZONTAL, {0, 0}, {800, 300}}
{
    // button.onHover = [&]() {
    //     std::cout << "Button hovered!" << std::endl;
    //     // lol
    //     int x, y;
    //     SDL_GetWindowPosition(graphics.window, &x, &y);
    //     SDL_SetWindowPosition(graphics.window, x+(rand()%10), y+(rand()%10));
    // };

    container.addComponent(UI::Button(graphics.shader, 0, "Fixed: 128", constants::fontHandler[4], 128));
    
    container.addComponent(UI::Button(graphics.shader, 0, "Auto", constants::fontHandler[4]));
    
    container.addComponent(UI::Button(graphics.shader, 0, "Fixed: 256", constants::fontHandler[4], 256));
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
