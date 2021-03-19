#pragma once
#include <vector>
#include "component.hpp"

namespace UI
{
    class ComponentList
    {
    protected:
        std::vector<UI::Component> components;
    public:
        ComponentList(const std::vector<UI::Component>& components = {})
            : components{components} {};
        virtual ~ComponentList() {};

        virtual void updateComponents() {};
        
        template <typename T>
        void addComponent(const T& component) {
            components.push_back(UI::Component{
                    component
                });
            updateComponents();
        }
    };
}
