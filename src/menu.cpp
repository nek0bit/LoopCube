#include "menu.hpp"

Menu::Menu(const Graphics& graphics)
    : button{graphics.shader, 0, "Yeah", constants::fontHandler.getFont(4), {50, 50}, 128}
{
    button.onHover = [&]() {
        std::cout << "Button hovered!" << std::endl;
        // lol
        int x, y;
        SDL_GetWindowPosition(graphics.window, &x, &y);
        SDL_SetWindowPosition(graphics.window, x+(rand()%10), y+(rand()%10));
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
