#pragma once
#include <variant>
#include "genericcomponent.hpp"

// Elements
#include "components.hpp"

namespace UI
{
    class ScrollList: public UI::GenericComponent,
                      public UI::ComponentList // Only needed for generic addComponent method and vector
    {
    private:
        UI::Scrollbar scrollbar;

        double translateComponentsY;
    public:
        ScrollList(const glm::ivec2& size = {SIZE_AUTO, SIZE_AUTO},
                   const glm::ivec2& position = {0, 0},
                   const Margin& margin = {0, 0, 0, 0});
        ~ScrollList();

        // Calls updateComponents()
        void refreshContent() override;

        // Updates each components place and size appropriately
        void updateComponents() override;
        
        void update(const Camera& camera, const EventWrapper& events) override;
        void draw(const Graphics& graphics, Transform transform = {}) const noexcept override;
    };
}
