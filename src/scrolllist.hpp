#pragma once
#include <variant>
#include "genericcomponent.hpp"
#include "component.hpp"
#include "componentlist.hpp"

namespace UI
{
    class ScrollList: public UI::GenericComponent,
                      public UI::ComponentList // Only needed for generic addComponent method and vector
    {
        ScrollList(const unsigned id,
             const glm::ivec2& size = {0, 0},
             const glm::ivec2& position = {0, 0});
        ~ScrollList();

        void updateComponents() override;

        void update(const Camera& camera, const EventWrapper& events) override;
        void draw(const Graphics& graphics, const Transform& transform = {}) const noexcept override;
    };
}
