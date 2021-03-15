#pragma once
#include <variant>

#include "componenttypes.hpp"
#include "button.hpp"

namespace UI
{
    struct Component
    {        
        std::variant<
            UI::Button
            > data;
    };
}
