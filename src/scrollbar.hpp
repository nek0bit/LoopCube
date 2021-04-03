#pragma once
#include <functional>
#include <glm/glm.hpp>
#include "genericcomponent.hpp"

namespace UI
{
    class Scrollbar: public GenericComponent
    {
    private:
        void fixScrollbar() noexcept;
        double lastMousePos;
    public:
        Scrollbar(const glm::ivec2& size = {SIZE_AUTO, SIZE_AUTO},
                  const glm::ivec2& position = {0, 0},
                  const Margin& margin = {0, 0, 0, 0});
        virtual ~Scrollbar();

        void update(const Camera& camera, const EventWrapper& events, Transform transform = {}) override;
        void draw(const Graphics& graphics, Transform transform = {}) const noexcept override;

        bool isClickingScrollbar(const EventWrapper& events, const Transform& transform = {}) const noexcept;

        double scrollPosition;
        double scrollScale;
        int fullHeight;
        float viewHeight;
        // If true then scrollbar will draw and respond to events
        bool scrollbarEnabled;
        bool isBeingDragged;

        // Events
        std::function<void(double, double)> isScrolled;
    };
}
