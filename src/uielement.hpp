#pragma once
#include <functional>
#include <variant>
#include <glm/glm.hpp>
#include "eventwrapper.hpp"
#include "graphics.hpp"

class UiComponent
{
    glm::ivec2 position;
    glm::ivec2 size;
    
public:
    UiComponent();
    ~UiComponent();

    void onClick(const std::function<void(void)>& func);
    void onEnter();
    void onExit();

    void update(const EventWrapper& events);
    void draw(const Graphics& graphics);
};
