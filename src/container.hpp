#pragma once
#include <variant>
#include "genericcomponent.hpp"

// Elements
#include "components.hpp"

enum container_layout
{
    CONTAINER_HORIZONTAL,
    CONTAINER_VERTICAL
};

namespace UI
{
    class Container: public UI::GenericComponent,
                     public UI::ComponentList
    {
    public:
        Container(const container_layout layout,
                  const glm::ivec2& size = {0, 0},
                  const glm::ivec2& position = {0, 0},
                  const Margin& margin = {0, 0, 0, 0});
        ~Container();

        void refreshContent() override;

        // Updates each components place and size appropriately
        void updateComponents() override;
        
        // For game loop
        void update(const Camera& camera, const EventWrapper& events, Transform transform = {}) override;
        void draw(const Graphics& graphics, Transform transform = {}) const noexcept override;

        container_layout layout;
    };
}
