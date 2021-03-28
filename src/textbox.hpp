#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "constants.hpp"
#include "genericcomponent.hpp"
#include "model.hpp"
#include "texture.hpp"
#include "generic.hpp"
#include "text.hpp"
#include "textbuffer.hpp"

namespace UI
{
    class Textbox: public GenericComponent
    {
    private:
        void generateMesh();
        void handleInputs(const EventWrapper& events);
        void updateCursor();

        Model model;
        Text textModel;
        bool isFocused;
        TextBuffer buffer;
        float cursorX;
    public:
        Textbox(const GLuint shader, const unsigned id, TTF_Font* font,
                const SDL_Color color, const int sizeX, const std::string& defaultText = "",
                const glm::ivec2& position = {0, 0});
        ~Textbox();

        inline const std::string& getText() const noexcept { return textModel.getText(); }

        void updateTextboxText();

        void refreshContent() override;
        void update(const Camera& camera, const EventWrapper& events) override;
        void draw(const Graphics& graphics, Transform transform = {}) const noexcept override;
    };
}
