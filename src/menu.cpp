#include "menu.hpp"

constexpr int MAIN_WIDTH = 300;

Menu::Menu(const Graphics& graphics)
    : mainContainer{0, CONTAINER_VERTICAL, {MAIN_WIDTH, 32*3}}
{
    createMainMenuComponents(graphics);
}

Menu::~Menu()
{}

void Menu::createMainMenuComponents(const Graphics& graphics)
{
    // Create other components
    // These components just stretch to list
    UI::Button playButton = UI::Button(graphics.shader, 0, "Play",
                                       constants::fontHandler[4], SIZE_AUTO);

    UI::Button configButton = UI::Button(graphics.shader, 1, "Config",
                                         constants::fontHandler[4], SIZE_AUTO);

    UI::Button quitButton = UI::Button(graphics.shader, 2, "Quit",
                                       constants::fontHandler[4], SIZE_AUTO);

    // Pack components
    mainContainer.addComponent(playButton);
    mainContainer.addComponent(configButton);
    mainContainer.addComponent(quitButton);
}

void Menu::update(const Camera& camera, const EventWrapper& events)
{
    // Center main list
    mainContainer.position.x = (camera.size.w / 2) - (mainContainer.size.x / 2);
    mainContainer.position.y = (camera.size.h / 2) - (mainContainer.size.y / 2);
    
    mainContainer.update(camera, events);
}

void Menu::render(const Graphics& graphics) const noexcept
{
    mainContainer.draw(graphics);
}
