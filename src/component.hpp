#pragma once
#include <variant>

enum component_t
{
    COMPONENT_BUTTON = 0,
    COMPONENT_TEXT,
    COMPONENT_CHECKBOX,
    COMPONENT_TEXTBOX,
    COMPONENT_CONTAINER_H,
    COMPONENT_CONTAINER_V
};

struct Component
{
    component_t type;
    std::variant<
        void,
        int
        > data;
};
