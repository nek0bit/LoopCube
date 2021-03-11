#include "button.hpp"

UI::Button::Button(const GLuint shader, const unsigned id,
                   const glm::ivec2& position, const glm::ivec2& size)
    : GenericComponent(id, position, size),
      model{shader}
{
    generateButtonMesh();
}

UI::Button::~Button()
{}

void UI::Button::generateButtonMesh()
{
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

    Generic::Render::generateSquare(mesh, 0.0f, 0.0f, size.x, size.y,
                                    tCoord[0].begX, tCoord[0].begY, tCoord[0].endX, tCoord[0].endY);
    
    Generic::Render::generateSquare(mesh, size.x, 0.0f, size.x*2, size.y,
                                    tCoord[1].begX, tCoord[1].begY, tCoord[1].endX, tCoord[1].endY);
    
    Generic::Render::generateSquare(mesh, size.x*2, 0.0f, size.x*3, size.y,
                                    tCoord[2].begX, tCoord[2].begY, tCoord[2].endX, tCoord[2].endY);

    model.setBufferData(mesh);
}

void UI::Button::update(const EventWrapper& events)
{}

void UI::Button::draw(const Graphics& graphics) const noexcept
{
    graphics.textures.getTexture(TEXTURE_UI_BUTTON)->bind();
    model.draw(graphics.uniforms.model, graphics.uniforms.tex, glm::vec3(position.x, position.y, 0.0f));
}
