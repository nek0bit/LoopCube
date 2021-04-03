#include "menu.hpp"

constexpr const char* GAME_TITLE = "LoopCube";
constexpr int MAIN_WIDTH = 300;
constexpr int MAIN_HEIGHT = 300;

Menu::Menu(const Graphics& graphics)
    : mainContainer{CONTAINER_HORIZONTAL,
                    {MAIN_WIDTH, MAIN_HEIGHT}}
{
    createMainMenuComponents(graphics);
}

Menu::~Menu()
{}

void Menu::createMainMenuComponents(const Graphics& graphics)
{
    UI::setShader(graphics.shader);
    /*
    // Create title text
    UI::setFont(constants::fontHandler[5]);
    UI::Margin titleMargin{0, MAIN_TITLE_OFFSET, 0, 0};
    UI::TextComponent titleText{"LoopCube", SDL_Color{255, 255, 255, 255},
                                {MAIN_WIDTH, SIZE_AUTO}, {}, titleMargin};

    // Create other components
    // These components just stretch to list
    UI::Margin buttonMargin{0, MARGIN_BOTTOM, 0, 0};
    UI::setFont(constants::fontHandler[4]);

    // Buttons
    UI::Button playButton{"Play", SIZE_AUTO, {}, buttonMargin};
    UI::Button configButton{"Config", SIZE_AUTO, {}, buttonMargin};
    UI::Button quitButton{"Quit", SIZE_AUTO, {}};

    // Add components (it packs automagically)
    mainContainer.addComponent(titleText);
    mainContainer.addComponent(playButton);
    mainContainer.addComponent(configButton);
    mainContainer.addComponent(quitButton);
    */
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
