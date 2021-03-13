#pragma once
#include "constants.hpp"
#include "genericcomponent.hpp"
#include "model.hpp"
#include "texture.hpp"
#include "generic.hpp"

namespace UI
{
    class Button: public GenericComponent
    {
    private:
        void generateButtonMesh();

        Model model;
    public:
        Button(const GLuint shader, const unsigned id,
               const glm::ivec2& position = {0, 0}, const int size = 0);
        ~Button();

        void update(const Camera& camera, const EventWrapper& events) override;
        void draw(const Graphics& graphics) const noexcept override;
    };
}
