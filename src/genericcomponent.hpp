#pragma once
#include <functional>
#include <variant>
#include <glm/glm.hpp>

#include "camera.hpp"
#include "generic.hpp"
#include "eventwrapper.hpp"
#include "graphics.hpp"
#include "transform.hpp"
#include "componenttypes.hpp"

enum component_size_t
{
    SIZE_AUTO = -1,
    SIZE_NONE
};

enum fixedsize_t
{
    FIXED_W = 1,
    FIXED_H
};

namespace UI
{
    class GenericComponent
    {
    private:
        // Event handling (click, enter, etc.)
        void eventClick(const Camera& camera, const EventWrapper& events);
        void eventHover(const Camera& camera, const EventWrapper& events);
    protected:
        [[nodiscard]] bool isVmouseTouching(const Camera& camera, const EventWrapper& events) const noexcept;
        void handleEvents(const Camera& camera, const EventWrapper& events); // Calls all event methods
    public:
        glm::ivec2 position;
        glm::ivec2 size;
        glm::ivec2 initialSize;
        
        GenericComponent(const component_t type,
                         const unsigned id,
                         const glm::ivec2& position = {0, 0},
                         const glm::ivec2& size = {0, 0});
        virtual ~GenericComponent();

        virtual void refreshContent();
        virtual void update(const Camera& camera, const EventWrapper& events);
        virtual void draw(const Graphics&, Transform = {}) const noexcept;

        component_t type;
        unsigned id;

        // Events
        std::function<void(void)> onClick;
        std::function<void(void)> onHover;
        
        // Properties
        glm::vec3 scale;

        uint8_t fixed;
    };
}
