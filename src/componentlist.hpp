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
        ComponentList()
            : components{} {};
        virtual ~ComponentList() {};

        virtual void updateComponents() {};
        
        template <typename T>
        void addComponent(const T& component) {
            components.push_back(component);
            updateComponents();
        }
    };
}
