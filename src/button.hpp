#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <memory>

#include "constants.hpp"
#include "genericcomponent.hpp"
#include "model.hpp"
#include "texture.hpp"
#include "generic.hpp"
#include "text.hpp"

namespace UI
{
    class Button: public GenericComponent
    {
    private:
        void generateButtonMesh();

        Model model;
        Text textModel;
    public:
        Button(const GLuint shader, const unsigned id, const std::string& text,
               TTF_Font* font, const glm::ivec2& position = {0, 0}, const int size = 0);
        ~Button();

        void setText(const std::string& text);
        inline const std::string& getText() const noexcept { return textModel.getText(); }

        void updateButtonText();

        void refreshContent() override;
        void update(const Camera& camera, const EventWrapper& events) override;
        void draw(const Graphics& graphics, const Transform& transform = {}) const noexcept override;
    };
}
