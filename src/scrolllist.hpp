#pragma once
#include <variant>
#include "genericcomponent.hpp"
#include "component.hpp"


// Elements
#include "button.hpp"
#include "scrollbar.hpp"
#include "container.hpp"


namespace UI
{
    class ScrollList: public UI::GenericComponent,
                      public UI::ComponentList // Only needed for generic addComponent method and vector
    {
    public:
        ScrollList(const unsigned id,
             const glm::ivec2& size = {0, 0},
             const glm::ivec2& position = {0, 0});
        ~ScrollList();

        void updateComponents() override;

        void update(const Camera& camera, const EventWrapper& events) override;
        void draw(const Graphics& graphics, const Transform& transform = {}) const noexcept override;
    };
}
