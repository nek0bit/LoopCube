#include "menu.hpp"

Menu::Menu(const Graphics& graphics)
    : container{0, {800, 600}, {0, 0}}
{
    container.addComponent(
        UI::Button(
            graphics.shader, 0, "Fixed: 128", constants::fontHandler[4], {SIZE_AUTO, 80}
            )
        );
    container.addComponent(
        UI::Button(
            graphics.shader, 0, "Fixed: 128", constants::fontHandler[4], {SIZE_AUTO, 40}
            )
        );

    container.addComponent(
        UI::Button(
            graphics.shader, 0, "Fixed: 128", constants::fontHandler[4], {SIZE_AUTO, 500}
            )
        );

    container.addComponent(
        UI::Button(
            graphics.shader, 0, "Fixed: 128", constants::fontHandler[4], {SIZE_AUTO, 300}
            )
        );
    container.addComponent(
        UI::Button(
            graphics.shader, 0, "Fixed: 128", constants::fontHandler[4], {SIZE_AUTO, 300}
            )
        );
    
   

    UI::Container miniContainer{0, CONTAINER_HORIZONTAL, {SIZE_AUTO, 120}};
    miniContainer.addComponent(
        UI::Button(
            graphics.shader, 1, "Button 1", constants::fontHandler[4], {SIZE_AUTO, SIZE_AUTO}
            )
        );

    miniContainer.addComponent(
        UI::Button(
            graphics.shader, 1, "Button 2", constants::fontHandler[4], {SIZE_AUTO, SIZE_AUTO}
            )
        );

    miniContainer.addComponent(
        UI::Button(
            graphics.shader, 1, "Button 3", constants::fontHandler[4], {SIZE_AUTO, SIZE_AUTO})
        );

    container.addComponent(miniContainer);


    container.addComponent(
        UI::Button(
            graphics.shader, 0, "Last", constants::fontHandler[4], {SIZE_AUTO, 300}
            )
        );

    container.addComponent(
            UI::TextComponent(
                graphics.shader, 1, "Last", 
                SDL_Color{255, 255, 255, 255}, constants::fontHandler[4], {SIZE_AUTO, SIZE_AUTO}
                )
            );
}

Menu::~Menu()
{}

void Menu::update(const Camera& camera, const EventWrapper& events)
{
    // button.update(camera, events);
    container.update(camera, events);
}

void Menu::render(const Graphics& graphics) const noexcept
{
    // button.draw(graphics);
    container.draw(graphics);
}
