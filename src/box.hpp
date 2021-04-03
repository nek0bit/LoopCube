#pragma once
#include "genericcomponent.hpp"
#include "model.hpp"

// Components
#include "component.hpp"
#include "components.hpp"

namespace UI
{
    class Box: public UI::GenericComponent
    {
    private:
        const int textureId;
        Model model;
        UI::Component component;
    public:
        Box(const int textureId,
            const UI::Component& component,
            const glm::ivec2& size = {SIZE_AUTO, SIZE_AUTO},
            const glm::ivec2& position = {0, 0},
            const Margin& margin = {0, 0, 0, 0});
        ~Box();

        template <typename T>
        void setComponent(const T& component) {
            this->component = component;
            refreshContent();
        }

        void refreshContent() override;

        void update(const Camera& camera, const EventWrapper& events, Transform transform = {}) override;
        void draw(const Graphics& graphics, Transform transform = {}) const noexcept override;
    };
}
