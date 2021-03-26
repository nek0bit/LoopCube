#pragma once
#include "graphics.hpp"
#include "eventwrapper.hpp"
#include "constants.hpp"

// I'm lazy for now
#include "components.hpp"

class Menu
{
private:
    UI::ScrollList container;
public:
    Menu(const Graphics& graphics);
    ~Menu();

    void update(const Camera& camera, const EventWrapper& events);
    void render(const Graphics& graphics) const noexcept;
};
