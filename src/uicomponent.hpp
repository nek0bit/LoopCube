#pragma once
#include <functional>
#include <optional>
#include <variant>
#include <glm/glm.hpp>
#include "eventwrapper.hpp"
#include "graphics.hpp"

class UiComponent
{
    const unsigned id;
    glm::ivec2 position;
    glm::ivec2 size;

    std::optional<std::function<void(void)>> clickFunc;
    std::optional<std::function<void(void)>> enterFunc;
    std::optional<std::function<void(void)>> exitFunc;
public:
    UiComponent(const int id, const glm::ivec2& position = {0, 0}, const glm::ivec2& size = {0, 0});
    ~UiComponent();

    void onClick(const std::function<void(void)>& clickFunctor);
    void onEnter(const std::function<void(void)>& enterFunctor);
    void onExit(const std::function<void(void)>& exitFunctor);

    void update(const EventWrapper& events);
    void draw(const Graphics& graphics);
};
