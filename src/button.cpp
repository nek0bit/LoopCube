#include "button.hpp"


// Size has fixed width
UI::Button::Button(const std::string& text, const int sizeX, const glm::ivec2& position)
    : GenericComponent(COMPONENT_BUTTON, position, {sizeX, 32}),
      model{UI::_ImmediateMode::_SHADER},
      // TODO don't hardcode color!
      textModel{UI::_ImmediateMode::_SHADER, text,
    SDL_Color{255, 255, 255, 255},
    UI::_ImmediateMode::_FONT}
{
    fixed = FIXED_H;
    generateButtonMesh();
}

UI::Button::~Button()
{}

void UI::Button::generateButtonMesh()
{
    constexpr float BUTTON_BL_W = 32.0f;
    const float BUTTON_MID_W = size.x - BUTTON_BL_W;
    const float BUTTON_END_W = BUTTON_MID_W + BUTTON_BL_W;
    std::vector<Vertex> mesh{};

    const TextureInfo tInfo = constants::textureInfo[TEXTURE_UI_BUTTON];

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
    Generic::Render::generateSquare(mesh, 0.0f, 0.0f, BUTTON_BL_W, size.y,
                                    tCoord[0].begX, tCoord[0].begY, tCoord[0].endX, tCoord[0].endY);

    // Don't bother drawing if too small
    if (size.x > BUTTON_BL_W * 2)
    {
        // Middle vertices
        Generic::Render::generateSquare(mesh, BUTTON_BL_W, 0.0f, BUTTON_MID_W, size.y,
                                        tCoord[1].begX, tCoord[1].begY, tCoord[1].endX, tCoord[1].endY);
    }

    // End vertices
    Generic::Render::generateSquare(mesh, BUTTON_MID_W, 0.0f, BUTTON_END_W, size.y,
                                    tCoord[2].begX, tCoord[2].begY, tCoord[2].endX, tCoord[2].endY);

    // Bind data
    model.setBufferData(mesh);

    updateButtonText();
}

void UI::Button::setText(const std::string& text)
{
    textModel.setText(text);
}

void UI::Button::refreshContent()
{
    generateButtonMesh();
}

void UI::Button::updateButtonText()
{
    const uint16_t offsetX = size.x / 2 - (textModel.size.x / 2);
    const uint16_t offsetY = size.y / 2 - (textModel.size.y / 2);
    textModel.scale = glm::vec3(scale.x, scale.y, 0.0f);
    textModel.position = glm::vec3(position.x + offsetX * scale.x,
                                   position.y + offsetY * scale.y, 0.0f);
}

void UI::Button::update(const Camera& camera, const EventWrapper& events)
{
    handleEvents(camera, events);
}

void UI::Button::draw(const Graphics& graphics, Transform transform) const noexcept
{
    graphics.textures.getTexture(TEXTURE_UI_BUTTON)->bind();
    model.draw(graphics.uniforms.model, graphics.uniforms.tex,
                glm::vec3(position.x, position.y, 0.0f) + transform.translate,
                scale + transform.scale);

    // Draw text
    textModel.draw(graphics, transform);
}
