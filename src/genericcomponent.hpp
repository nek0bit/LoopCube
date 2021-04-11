#pragma once
#include <functional>
#include <variant>
#include <glm/glm.hpp>
#include <cmath>

#include "margin.hpp"
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

// TODO use structs for arguments that get repeated a lot

namespace UI
{
    class GenericComponent
    {
    private:
        // Event handling (click, enter, etc.)
        void eventClick(const Camera& camera, const EventWrapper& events, const Transform& transform);
        void eventHover(const Camera& camera, const EventWrapper& events, const Transform& transform);
    protected:
        [[nodiscard]] bool isVmouseTouching(const Camera& camera, const EventWrapper& events, const Transform& transform) const noexcept;
        void handleEvents(const Camera& camera, const EventWrapper& events, const Transform& transform); // Calls all event methods
        void transformPropify(Transform& transform) noexcept;
    public:
        glm::ivec2 position;
        glm::ivec2 realPosition;
        glm::ivec2 size;
        glm::ivec2 initialSize;
        
        GenericComponent(const component_t type,
                         const glm::ivec2& position = {0, 0},
                         const glm::ivec2& size = {0, 0},
                         const Margin& margin = {0, 0, 0, 0});
        virtual ~GenericComponent();

        virtual void refreshContent();
        virtual void update(const Camera& camera, const EventWrapper& events, Transform = {});
        virtual void draw(const Graphics&, Transform = {}) const noexcept;

        component_t type;
        unsigned id;

        // Events
        std::function<void(void)> onClick;
        std::function<void(void)> onHover;
        
        // Properties
        Margin margin;
        glm::vec3 scale;

        uint8_t fixed;

        // Any type can go here, but containers are recommended
        GenericComponent* parentContainer;
    };
}
