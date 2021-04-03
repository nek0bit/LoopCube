#include "menu.hpp"

constexpr const char* GAME_TITLE = "LoopCube";
constexpr int MAIN_WIDTH = 500;
constexpr int MAIN_HEIGHT = 400;
constexpr int BUTTON_MARGIN_BOTTOM = 10;

Menu::Menu(const Graphics& graphics)
    : mainContainer{CONTAINER_HORIZONTAL,
                    {MAIN_WIDTH, MAIN_HEIGHT}},
      leftPane{},
      rightPane{}
{
    createMainMenuComponents(graphics);
}

Menu::~Menu()
{}

void Menu::createMainMenuComponents(const Graphics& graphics)
{
    UI::setShader(graphics.shader);

    // Margins
    UI::Margin titleMargin{0, 0, 0, 0};
    UI::Margin buttonMargin{0, BUTTON_MARGIN_BOTTOM, 0, 0};

    // Create title text
    UI::setFont(constants::fontHandler[5]);
    UI::TextComponent titleText{"LoopCube", SDL_Color{255, 255, 255, 255},
                                {SIZE_AUTO, SIZE_AUTO}, {}, titleMargin};

    // Create other components
    // Buttons
    UI::setFont(constants::fontHandler[4]);
    UI::Button playButton{"Play", SIZE_AUTO, {}, buttonMargin};
    UI::Button configButton{"Config", SIZE_AUTO, {}, buttonMargin};
    UI::Button quitButton{"Quit", SIZE_AUTO, {}};

    // Add components to left pane
    leftPane.addComponent(titleText);

    // Add components to right pane (it packs automagically)
    rightPane.addComponent(playButton);
    rightPane.addComponent(configButton);
    rightPane.addComponent(quitButton);

    // Add panes
    mainContainer.addComponent(leftPane);
    mainContainer.addComponent(rightPane);
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
