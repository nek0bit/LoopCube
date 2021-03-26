#pragma once
#include <variant>

namespace UI
{
    class Container;
    class Button;
    class Scrollbar;
    class ScrollList;
    class TextComponent;

    using Component = std::variant<
        UI::Container,
        UI::Button,
        UI::Scrollbar,
        UI::ScrollList,
        UI::TextComponent
    >;
}
