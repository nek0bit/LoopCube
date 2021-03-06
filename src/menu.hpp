#pragma once
#include <functional>
#include "immediate.hpp"
#include "graphics.hpp"
#include "eventwrapper.hpp"
#include "constants.hpp"

// I'm lazy for now
#include "components.hpp"

class Menu
{
private:
    UI::Container mainContainer;

    // Panes
    UI::ScrollList leftPane;
    UI::ScrollList rightPane;

    UI::Box box;
public:
    Menu(const Graphics& graphics);
    ~Menu();

    // Creates main menu components such as play button, quit button, etc.
    void createMainMenuComponents(const Graphics& graphics);
    
    // Generic game loop stuff
    void update(const Camera& camera, const EventWrapper& events);
    void render(const Graphics& graphics) const noexcept;

    std::function<void(void)> onPlayClick;
};
