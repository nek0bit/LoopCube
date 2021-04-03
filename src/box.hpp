#pragma once
#include <memory>
#include "genericcomponent.hpp"
#include "model.hpp"

// Components
#include "components.hpp"

namespace UI
{
    class Box: public UI::GenericComponent
    {
    private:
        int textureId;
        Model model;
        std::shared_ptr<UI::GenericComponent> component;
    public:
        Box(const int textureId,
            const glm::ivec2& size = {SIZE_AUTO, SIZE_AUTO},
            const glm::ivec2& position = {0, 0},
            const Margin& margin = {0, 0, 0, 0});
        ~Box();

        template <typename T>
        void setComponent(const T& component) {
            this->component = std::make_shared<T>(component);
            refreshContent();
        }

        void refreshContent() override;

        void update(const Camera& camera, const EventWrapper& events, Transform transform = {}) override;
        void draw(const Graphics& graphics, Transform transform = {}) const noexcept override;
    };
}
