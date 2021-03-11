#pragma once
#include <functional>
#include <optional>
#include <variant>
#include <glm/glm.hpp>

#include "eventwrapper.hpp"
#include "graphics.hpp"

namespace UI {
    class GenericComponent
    {
    protected:
        const unsigned id;
        glm::ivec2 position;
        glm::ivec2 size;

        std::optional<std::function<void(void)>> clickFunctor;
        std::optional<std::function<void(void)>> enterFunctor;
        std::optional<std::function<void(void)>> exitFunctor;
    public:
        GenericComponent(const unsigned id, const glm::ivec2& position = {0, 0}, const glm::ivec2& size = {0, 0});
        virtual ~GenericComponent();

        void onClick(const std::function<void(void)>& clickFunctor) noexcept;
        void onEnter(const std::function<void(void)>& enterFunctor) noexcept;
        void onExit(const std::function<void(void)>& exitFunctor) noexcept;

        virtual void update(const EventWrapper& events);
        virtual void draw(const Graphics& graphics) const noexcept;
    };
}
