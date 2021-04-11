#include "menu.hpp"

constexpr const char* GAME_TITLE = "LoopCube";
constexpr int MAIN_WIDTH = 600;
constexpr int MAIN_HEIGHT = 400;
constexpr int BUTTON_MARGIN_BOTTOM = 6;
constexpr int BUTTON_MARGIN_SIDE = 0;

Menu::Menu(const Graphics& graphics)
    : mainContainer{CONTAINER_HORIZONTAL,
                    {MAIN_WIDTH, MAIN_HEIGHT}},
      leftPane{{200, SIZE_AUTO}, {}, {15, 15, 15, 15}},
      rightPane{{SIZE_AUTO, SIZE_AUTO}, {}, {15, 15, 15, 15}},
      box{(UI::setShader(graphics.shader), TEXTURE_BOX), {MAIN_WIDTH, MAIN_HEIGHT}}
{
    createMainMenuComponents(graphics);
}

Menu::~Menu()
{}

void Menu::createMainMenuComponents(const Graphics& graphics)
{
    UI::setShader(graphics.shader);

    // Margins
    UI::Margin titleMargin{15, 10, 0, 0};
    UI::Margin buttonMargin{0, BUTTON_MARGIN_BOTTOM, BUTTON_MARGIN_SIDE, BUTTON_MARGIN_SIDE};

    // Create text
    UI::setFont(constants::fontHandler[5]);
    UI::TextComponent titleText{"LoopCube", SDL_Color{255, 255, 255, 255},
                                {SIZE_AUTO, SIZE_AUTO}, {}, titleMargin};
    UI::setFont(constants::fontHandler[3]);
    UI::TextComponent versionText{"Release ?", SDL_Color{155, 155, 155, 255},
                                  {SIZE_AUTO, SIZE_AUTO}, {}, {35, 0, 0, 0}};

    // Create other components
    // Buttons
    UI::setFont(constants::fontHandler[4]);
    UI::Button playButton{"Play", SIZE_AUTO, {}, buttonMargin};
    playButton.onClick = [&](){
        onPlayClick();
    };
    UI::Button configButton{"Config", SIZE_AUTO, {}, buttonMargin};
    UI::Button config1Button{"Config", SIZE_AUTO, {}, buttonMargin};
    UI::Button config2Button{"Config", SIZE_AUTO, {}, buttonMargin};
    UI::Button config3Button{"Config", SIZE_AUTO, {}, buttonMargin};
    UI::Button config7Button{"Config", SIZE_AUTO, {}, buttonMargin};
    UI::Button config8Button{"Config", SIZE_AUTO, {}, buttonMargin};
    UI::Button config9Button{"Config", SIZE_AUTO, {}, buttonMargin};
    UI::Button config4Button{"Config", SIZE_AUTO, {}, buttonMargin};
    UI::Button config5Button{"Config", SIZE_AUTO, {}, buttonMargin};
    UI::Button config6Button{"Config", SIZE_AUTO, {}, buttonMargin};
    UI::Button quitButton{"Quit", SIZE_AUTO, {}, buttonMargin};

    // Creates a line between both panes
    UI::TextureComponent paneSeperator{TEXTURE_SEPERATOR, {1, SIZE_AUTO}, {}, {0, 0, 15, 15}};

    // Add components to right pane (it packs automagically)
    leftPane.addComponent(playButton);
    leftPane.addComponent(configButton);
    leftPane.addComponent(config1Button);
    leftPane.addComponent(config2Button);
    leftPane.addComponent(config3Button);
    leftPane.addComponent(config4Button);
    leftPane.addComponent(config5Button);
    leftPane.addComponent(config6Button);
    leftPane.addComponent(config7Button);
    leftPane.addComponent(config8Button);
    leftPane.addComponent(config9Button);
    leftPane.addComponent(quitButton);

    // Add components to left pane
    rightPane.addComponent(titleText);
    rightPane.addComponent(versionText);

    // Add panes
    mainContainer.addComponent(leftPane);
    mainContainer.addComponent(paneSeperator);
    mainContainer.addComponent(rightPane);

    box.setComponent(mainContainer);
}

void Menu::update(const Camera& camera, const EventWrapper& events)
{
    // Center main list
    box.position.x = (camera.size.w / 2) - (mainContainer.size.x / 2);
    box.position.y = (camera.size.h / 2) - (mainContainer.size.y / 2);

    // mainContainer.update(camera, events);
    box.update(camera, events);
}

void Menu::render(const Graphics& graphics) const noexcept
{
    // mainContainer.draw(graphics);
    box.draw(graphics);
}
