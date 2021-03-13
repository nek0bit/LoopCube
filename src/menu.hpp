#pragma once
#include "graphics.hpp"
#include "eventwrapper.hpp"
#include "button.hpp"

class Menu
{
private:
    UI::Button button;
public:
    Menu(const Graphics& graphics);
    ~Menu();

    void update(const Camera& camera, const EventWrapper& events);
    void render(const Graphics& graphics) const noexcept;
};
