#pragma once
#include <variant>
#include "genericcomponent.hpp"
#include "component.hpp"

namespace UI
{
    class Container: public GenericComponent
    {
    private:
        std::vector<UI::Component> components;
    public:
        Container(const unsigned id,
                  const glm::ivec2& position = {0, 0},
                  const glm::ivec2& size = {0, 0});
        ~Container();

        void update(const Camera& camera, const EventWrapper& events) override;
        void draw(const Graphics& graphics, const Transform& transform = {}) const noexcept override;

        template <typename T>
        void addComponent(const T& component) {            
            components.push_back(UI::Component{
                    component
                });
        }
    };
}
