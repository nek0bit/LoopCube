#pragma once
#include "graphics.hpp"
#include "eventwrapper.hpp"
#include "container.hpp"
#include "button.hpp"
#include "constants.hpp"

class Menu
{
private:
    UI::Container container;
public:
    Menu(const Graphics& graphics);
    ~Menu();

    void update(const Camera& camera, const EventWrapper& events);
    void render(const Graphics& graphics) const noexcept;
};
