#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <memory>

#include "immediate.hpp"
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
        Button(const std::string& text, const int sizeX, const glm::ivec2& position = {0, 0});
        ~Button();

        void setText(const std::string& text);
        inline const std::string& getText() const noexcept { return textModel.getText(); }

        void updateButtonText();

        void refreshContent() override;
        void update(const Camera& camera, const EventWrapper& events) override;
        void draw(const Graphics& graphics, Transform transform = {}) const noexcept override;
    };
}
