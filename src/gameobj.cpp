#include "gameobj.hpp"

GameObject::GameObject(int modelId, int textureId, glm::vec3 position, glm::vec2 size)
    : position{position},
      size{size},
      textureId{textureId},
      src{0, 0, 0, 0}
{}

#ifndef __HEADLESS
void GameObject::render(const Graphics& graphics, const Camera& camera) const
{
    const glm::vec3 val = position + camera.position;
	SDL_Rect dest{static_cast<int>(val.x),
        static_cast<int>(val.y),
        static_cast<int>(size.x),
        static_cast<int>(size.y)};
    //SDL_RenderCopy(renderer, textures.getTexture(textureId)->texture, &src, &dest);
}

CollisionInfo GameObject::isColliding(const GameObject &obj2) const
{
	if (position.x < obj2.position.x + obj2.size.x &&
		position.x + size.x > obj2.position.x &&
		position.y < obj2.position.y + obj2.size.y &&
		position.y + size.y > obj2.position.y) {
		const double topCalc = (position.y+size.y) - obj2.position.y;
		const double bottomCalc = (obj2.position.y+obj2.size.y) - position.y;
		const double leftCalc = (position.x+size.x) - obj2.position.x;
		const double rightCalc = (obj2.position.x+obj2.size.y) - position.x;

		CollisionInfo info{-1, -1, -1, -1};
		// top collision
		if (topCalc < bottomCalc) {
			info.top = topCalc;
		}

		// bottom collision
		if (topCalc > bottomCalc) {
			info.bottom = bottomCalc;
		}

		// left collision
		if (leftCalc < rightCalc) {
			info.left = leftCalc;
		}

		// right collision
		if (leftCalc > rightCalc) {
			info.right = rightCalc;
		}
		return info;
	}
	return CollisionInfo{};
}

// Used for rendering culling
bool GameObject::shouldCull(const Camera& camera) const
{
    glm::vec3 val = position + camera.position;
    return (val.x + size.x < 0 || val.y + size.y < 0) // Upper-left culling
        || (val.x - size.x > camera.size.h || val.y - size.y > camera.size.h); // Bottom-right culling
}

#endif


void GameObject::update()
{
	src.x = 0;
	src.y = 0;
	src.w = size.x;
    src.h = size.y;
}
