#pragma once
#include <string>
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
            TextComponent(const GLuint shader,
                    const unsigned id,
                    const std::string& text,
                    SDL_Color color,
                    TTF_Font* font,
                    const glm::ivec2& size = {SIZE_AUTO, SIZE_AUTO},
                    const glm::ivec2& position = {0, 0});
            ~TextComponent();

            void setText(const std::string& text);
            inline const std::string& getText() const noexcept { return textModel.getText(); }

            void updateButtonText();

            void refreshContent() override;
            void update(const Camera& camera, const EventWrapper& events) override;
            void draw(const Graphics& graphics, Transform transform = {}) const noexcept override;
    };
}
