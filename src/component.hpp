#pragma once
#include <variant>

namespace UI
{
    class Container;
    class Button;
    class Scrollbar;
    class ScrollList;
    class TextComponent;
    class Textbox;
    class Box;
    class TextureComponent;

    using Component = std::variant<
        UI::Container,
        UI::Button,
        UI::Scrollbar,
        UI::ScrollList,
        UI::TextComponent,
        UI::Textbox,
        UI::Box,
        UI::TextureComponent
    >;
}
