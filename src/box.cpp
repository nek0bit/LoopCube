#include "box.hpp"

UI::Box::Box(const int textureId,
             const glm::ivec2& size,
             const glm::ivec2& position,
             const Margin& margin)
    : GenericComponent{COMPONENT_BOX, position, size, margin},
      textureId{textureId},
      model{UI::_ImmediateMode::_SHADER},
      component{nullptr}
{
    refreshContent();
}

UI::Box::~Box()
{}

void UI::Box::refreshContent()
{
    constexpr float T_W = constants::textureInfo[TEXTURE_BOX].tilemapX;
    constexpr float T_H = constants::textureInfo[TEXTURE_BOX].tilemapY;
    std::vector<Vertex> mesh{};

    const TextureInfo tInfo = constants::textureInfo[TEXTURE_BOX];

    const texcoord_info info = {
        static_cast<unsigned>(tInfo.sizeX),
        static_cast<unsigned>(tInfo.sizeY),
        static_cast<unsigned>(tInfo.tilemapX),
        static_cast<unsigned>(tInfo.tilemapY)
    };
    
    constexpr size_t T_COORD_SIZE = 9;
    const texcoord_t tCoord[T_COORD_SIZE] = {
        Texture::getTilemapCoord(info, 0, 0),
        Texture::getTilemapCoord(info, 1, 0),
        Texture::getTilemapCoord(info, 2, 0),
        Texture::getTilemapCoord(info, 0, 1),
        Texture::getTilemapCoord(info, 1, 1),
        Texture::getTilemapCoord(info, 2, 1),
        Texture::getTilemapCoord(info, 0, 2),
        Texture::getTilemapCoord(info, 1, 2),
        Texture::getTilemapCoord(info, 2, 2),
    };

    // Begin vertices
    const float midWidth = size.x;
    const float midHeight = size.y;

    // Oh this is bad
    // TOP
    Generic::Render::generateSquare(mesh, 0.0f, 0.0f, T_W, T_H,
                                    tCoord[0].begX, tCoord[0].begY, tCoord[0].endX, tCoord[0].endY);
    Generic::Render::generateSquare(mesh, T_W, 0.0f, midWidth - T_W, T_H,
                                    tCoord[1].begX, tCoord[1].begY, tCoord[1].endX, tCoord[1].endY);
    Generic::Render::generateSquare(mesh, midWidth - T_W, 0.0f, midWidth, T_H,
                                    tCoord[2].begX, tCoord[2].begY, tCoord[2].endX, tCoord[2].endY);
    // MIDDLE
    Generic::Render::generateSquare(mesh, 0.0f, T_H, T_W, midHeight - T_H,
                                    tCoord[3].begX, tCoord[3].begY, tCoord[3].endX, tCoord[3].endY);
    Generic::Render::generateSquare(mesh, T_W, T_H, midWidth - T_W, midHeight - T_H,
                                    tCoord[4].begX, tCoord[4].begY, tCoord[4].endX, tCoord[4].endY);
    Generic::Render::generateSquare(mesh, midWidth - T_W, T_H, midWidth, midHeight - T_H,
                                    tCoord[5].begX, tCoord[5].begY, tCoord[5].endX, tCoord[5].endY);

    // BOTTOM
    Generic::Render::generateSquare(mesh, 0.0f, midHeight - T_H, T_W, midHeight,
                                    tCoord[6].begX, tCoord[6].begY, tCoord[6].endX, tCoord[6].endY);
    Generic::Render::generateSquare(mesh, T_W, midHeight - T_H, midWidth - T_W, midHeight,
                                    tCoord[7].begX, tCoord[7].begY, tCoord[7].endX, tCoord[7].endY);
    Generic::Render::generateSquare(mesh, midWidth - T_W, midHeight - T_H, midWidth, midHeight,
                                    tCoord[8].begX, tCoord[8].begY, tCoord[8].endX, tCoord[8].endY);

    // Bind data
    model.setBufferData(mesh);
    if (component) component->refreshContent();
}

void UI::Box::update(const Camera& camera, const EventWrapper& events, Transform transform)
{
    transform.translate.x += position.x;
    transform.translate.y += position.y;
    if (component) component->update(camera, events, transform);
}

void UI::Box::draw(const Graphics& graphics, Transform transform) const noexcept
{

    graphics.textures.getTexture(textureId)->bind();
    model.draw(graphics.uniforms.model, graphics.uniforms.tex,
                glm::vec3(position.x, position.y, 0.0f) + transform.translate,
                scale + transform.scale);

    transform.translate.x += position.x;
    transform.translate.y += position.y;

    if (component) component->draw(graphics, transform);
}
