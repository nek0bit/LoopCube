#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "constants.hpp"
#include "genericcomponent.hpp"
#include "model.hpp"
#include "texture.hpp"
#include "generic.hpp"
#include "text.hpp"

namespace UI
{
    class Textbox: public GenericComponent
    {
    private:
        void generateMesh();

        Model model;
        Text textModel;
    public:
        Textbox(const GLuint shader, const unsigned id, const std::string& text,
                TTF_Font* font, const SDL_Color color, const glm::ivec2& size = {SIZE_AUTO, SIZE_AUTO},
                const glm::ivec2& position = {0, 0});
        ~Textbox();

        inline const std::string& getText() const noexcept { return textModel.getText(); }

        void updateTextboxText();

        void refreshContent() override;
        void update(const Camera& camera, const EventWrapper& events) override;
        void draw(const Graphics& graphics, Transform transform = {}) const noexcept override;
    };
}
