#include "box.hpp"

UI::Box::Box(const int textureId,
             const glm::ivec2& size,
             const glm::ivec2& position,
             const Margin& margin)
    : UI::GenericComponent{COMPONENT_BOX, size, position, margin},
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
    float j = 0.0f;
    for (size_t i = 0; i < T_COORD_SIZE; (i += 3, j += T_H))
    {
        Generic::Render::generateSquare(mesh, 0.0f, j, T_W, T_H,
                                        tCoord[i].begX, tCoord[i].begY, tCoord[i].endX, tCoord[i].endY);
        Generic::Render::generateSquare(mesh, T_W, j, T_W, T_H,
                                        tCoord[i+1].begX, tCoord[i+1].begY, tCoord[i+1].endX, tCoord[i+1].endY);
        Generic::Render::generateSquare(mesh, T_W * 2, j, T_W, T_H,
                                        tCoord[i+2].begX, tCoord[i+2].begY, tCoord[i+2].endX, tCoord[i+2].endY);
    }

    // Bind data
    model.setBufferData(mesh);
}

void UI::Box::update(const Camera& camera, const EventWrapper& events, Transform transform)
{
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
