#pragma once

#include "immediate.hpp"
#include "genericcomponent.hpp"

namespace UI
{
    class TextureComponent: public GenericComponent
    {
    public:
        TextureComponent(const size_t textureId,
                         const glm::ivec2& size = {SIZE_AUTO, SIZE_AUTO},
                         const glm::ivec2& position = {0, 0},
                         const Margin& margin = {0, 0, 0, 0});
        ~TextureComponent();

        void refreshContent() override;
        void update(const Camera& camera, const EventWrapper& events, Transform = {}) override;
        void draw(const Graphics& graphics, Transform transform = {}) const noexcept override;

        size_t textureId;
    };
}
