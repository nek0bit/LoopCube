#pragma once
#include <glm/glm.hpp>
#include "genericcomponent.hpp"

namespace UI
{
    class Scrollbar: public GenericComponent
    {
    public:
        Scrollbar(const unsigned id,
                  int fullHeight,
                  int viewHeight,
                  const glm::ivec2& size = {SIZE_AUTO, SIZE_AUTO},
                  const glm::ivec2& position = {0, 0});
        virtual ~Scrollbar();

        void update(const Camera& camera, const EventWrapper& events) override;
        void draw(const Graphics& graphics, const Transform& transform = {}) const noexcept override;
        
        int fullHeight;
        int viewHeight;
    };
}
