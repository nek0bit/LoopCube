#pragma once
#include <string>

#include "immediate.hpp"
#include "componenttypes.hpp"
#include "genericcomponent.hpp"
#include "text.hpp"

namespace UI
{
    class TextComponent: public GenericComponent
    {
    private:
        Text textModel;
    public:
        TextComponent(const std::string& text,
                      SDL_Color color,
                      const glm::ivec2& size = {SIZE_AUTO, SIZE_AUTO},
                      const glm::ivec2& position = {0, 0},
                      const Margin& margin = {0, 0, 0, 0});
        ~TextComponent();

        void setText(const std::string& text);
        inline const std::string& getText() const noexcept { return textModel.getText(); }

        void updateButtonText();

        void refreshContent() override;
        void update(const Camera& camera, const EventWrapper& events) override;
        void draw(const Graphics& graphics, Transform transform = {}) const noexcept override;
    };
}
