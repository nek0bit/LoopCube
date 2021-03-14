#pragma once
#include "genericcomponent.hpp"
#include "component.hpp"

class Container
{
private:
    std::vector<UI::Component> components;
public:
    Container(const unsigned id,
              const glm::ivec2& position = {0, 0},
              const glm::ivec2& size = {0, 0});
    ~Container();

    template <typename T>
    void addComponent(const T& component) {
        const component_t type = component.type;

        components.emplace_back(UI::Component{
                type,
                component
            });
    }
};
