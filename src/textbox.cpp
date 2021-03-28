// Quite a bit of code borrowed from button.cpp here...

#include "textbox.hpp"

UI::Textbox::Textbox(const GLuint shader, const unsigned id, TTF_Font* font,
        const SDL_Color color, const int sizeX, const std::string& defaultText, const glm::ivec2& position)
    : GenericComponent(COMPONENT_TEXTBOX, id, position, {sizeX, 32}),
      model{shader},
      textModel{shader, defaultText, color, font},
      isFocused{false},
      buffer{}
{
    generateMesh();

    updateTextboxText();
}

UI::Textbox::~Textbox()
{}

void UI::Textbox::generateMesh()
{
    constexpr float TEXTBOX_BL_W = 32.0f;
    const float TEXTBOX_MID_W = size.x - TEXTBOX_BL_W;
    const float TEXTBOX_END_W = TEXTBOX_MID_W + TEXTBOX_BL_W;
    std::vector<Vertex> mesh{};

    const TextureInfo tInfo = constants::textureInfo[TEXTURE_UI_TEXTBOX];

    const texcoord_info info = {
        static_cast<unsigned>(tInfo.sizeX),
        static_cast<unsigned>(tInfo.sizeY),
        static_cast<unsigned>(tInfo.tilemapX),
        static_cast<unsigned>(tInfo.tilemapY)
    };

    const texcoord_t tCoord[3] = {
        Texture::getTilemapCoord(info, 0, 0),
        Texture::getTilemapCoord(info, 1, 0),
        Texture::getTilemapCoord(info, 2, 0)
    };

    // Begin vertices
    Generic::Render::generateSquare(mesh, 0.0f, 0.0f, TEXTBOX_BL_W, size.y,
                                    tCoord[0].begX, tCoord[0].begY, tCoord[0].endX, tCoord[0].endY);

    // Don't bother drawing if too small
    if (size.x > TEXTBOX_BL_W * 2)
    {
        // Middle vertices
        Generic::Render::generateSquare(mesh, TEXTBOX_BL_W, 0.0f, TEXTBOX_MID_W, size.y,
                                        tCoord[1].begX, tCoord[1].begY, tCoord[1].endX, tCoord[1].endY);
    }

    // End vertices
    Generic::Render::generateSquare(mesh, TEXTBOX_MID_W, 0.0f, TEXTBOX_END_W, size.y,
                                    tCoord[2].begX, tCoord[2].begY, tCoord[2].endX, tCoord[2].endY);

    // Bind data
    model.setBufferData(mesh);

    updateTextboxText();
}

void UI::Textbox::refreshContent()
{
    generateMesh();
}

void UI::Textbox::updateTextboxText()
{
    // Midpoint for text
    const uint16_t midY = size.y / 2 - (textModel.size.y / 2);
    constexpr uint16_t X_BEGIN = 16;
    textModel.scale = glm::vec3(scale.x, scale.y, 0.0f);
    textModel.position = glm::vec3(position.x + X_BEGIN * scale.x,
                                   position.y + midY * scale.y, 0.0f);
}

void UI::Textbox::update(const Camera& camera, const EventWrapper& events)
{
    const bool touching = isVmouseTouching(camera, events);
    handleEvents(camera, events);

    switch (events.vmouse.clicked)
    {
    case 1:
        if (!isFocused && touching)
        {
            isFocused = true;
        }
        else if (isFocused && events.vmouse.clicked == 1 && !touching)
        {
            isFocused = false;
        }

        break;
    default: break;
    }

    if (isFocused && events.textChar != NULL)
    {
        handleInputs(events);
    }

    // TODO clean this up
    // Movement
    if (events.keyState[18])
    {
        buffer.moveCursor(-1);
    } else if (events.keyState[19])
    {
        buffer.moveCursor(1);
    }
}

void UI::Textbox::handleInputs(const EventWrapper& events)
{
    buffer.insert(events.textChar);
    textModel.setText(buffer.getText());
}

void UI::Textbox::draw(const Graphics& graphics, Transform transform) const noexcept
{
    graphics.textures.getTexture(TEXTURE_UI_TEXTBOX)->bind();
    model.draw(graphics.uniforms.model, graphics.uniforms.tex,
            glm::vec3(position.x, position.y, 0.0f) + transform.translate,
            scale + transform.scale);

    // Draw text
    textModel.draw(graphics, transform);
}
