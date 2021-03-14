#pragma once
#include <variant>

#include "componenttypes.hpp"
#include "button.hpp"

namespace UI
{
    struct Component
    {
        component_t type;
        std::variant<
            UI::Button
            > data;
    };
}
