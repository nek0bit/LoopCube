#pragma once
#include <variant>
#include "genericcomponent.hpp"
#include "component.hpp"
#include "componentlist.hpp"

namespace UI
{
    class List: public UI::GenericComponent,
                public UI::ComponentList // Only needed for generic addComponent method and vector
    {
        List(const unsigned id,
             const glm::ivec2& size = {0, 0},
             const glm::ivec2& position = {0, 0});
        ~List();

        void updateComponents() override;

        void update(const Camera& camera, const EventWrapper& events) override;
        void draw(const Graphics& graphics, const Transform& transform = {}) const noexcept override;
    };
}
