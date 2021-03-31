#pragma once
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
public:
    Menu(const Graphics& graphics);
    ~Menu();

    // Creates main menu components such as play button, quit button, etc.
    void createMainMenuComponents(const Graphics& graphics);
    
    // Generic game loop stuff
    void update(const Camera& camera, const EventWrapper& events);
    void render(const Graphics& graphics) const noexcept;
};
