#pragma once
#include <functional>
#include <optional>
#include <variant>
#include <glm/glm.hpp>

#include "camera.hpp"
#include "generic.hpp"
#include "eventwrapper.hpp"
#include "graphics.hpp"

namespace UI {
    class GenericComponent
    {
    private:
        // Event handling (click, enter, etc.)
        [[nodiscard]] bool isVmouseTouching(const Camera& camera, const EventWrapper& events) const noexcept;
        void eventClick(const Camera& camera, const EventWrapper& events);
    protected:
        const unsigned id;
        glm::ivec2 position;
        glm::ivec2 size;
        
        void handleEvents(const Camera& camera, const EventWrapper& events); // Calls all event methods

        std::optional<std::function<void(unsigned)>> clickFunctor;
        /*
        std::optional<std::function<void(void)>> enterFunctor;
        std::optional<std::function<void(void)>> exitFunctor;
        */
    public:
        GenericComponent(const unsigned id, const glm::ivec2& position = {0, 0}, const glm::ivec2& size = {0, 0});
        virtual ~GenericComponent();

        void onClick(const std::function<void(unsigned)>& clickFunctor) noexcept;
        /*
        void onEnter(const std::function<void(void)>& enterFunctor) noexcept;
        void onExit(const std::function<void(void)>& exitFunctor) noexcept;
        */

        virtual void update(const Camera& camera, const EventWrapper& events);
        virtual void draw(const Graphics& graphics) const noexcept;
    };
}
