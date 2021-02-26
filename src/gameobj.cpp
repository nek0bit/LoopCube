#include "gameobj.hpp"

GameObject::GameObject(int textureId, double x, double y, double w, double h)
    : position{x, y},
      size{w, h},
      textureId{textureId},
      src{0, 0, 0, 0}
{}

#ifndef __HEADLESS
Vec2 GameObject::getPos(const Camera& camera) const
{
	return position/* + camera*/;
}

void GameObject::render(const Graphics& graphics, TextureHandler& textures, const Camera& camera) const
{
    const Vec2 val = position/* + camera*/;
	SDL_Rect dest{static_cast<int>(val.x),
        static_cast<int>(val.y),
        static_cast<int>(size.w),
        static_cast<int>(size.h)};
    //SDL_RenderCopy(renderer, textures.getTexture(textureId)->texture, &src, &dest);
}

CollisionInfo GameObject::isColliding(const GameObject &obj2) const
{    
	if (position.x < obj2.position.x + obj2.size.w &&
		position.x + size.w > obj2.position.x &&
		position.y < obj2.position.y + obj2.size.h &&
		position.y + size.h > obj2.position.y) {
		const double topCalc = (position.y+size.h) - obj2.position.y;
		const double bottomCalc = (obj2.position.y+obj2.size.h) - position.y;
		const double leftCalc = (position.x+size.w) - obj2.position.x;
		const double rightCalc = (obj2.position.x+obj2.size.h) - position.x;

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
    const Vec2 val = position/* + camera*/;
    return (val.x + size.w < 0 || val.y + size.h < 0) // Upper-left culling
        || (val.x - size.w > camera.size.h || val.y - size.h > camera.size.h); // Bottom-right culling
}

#endif


void GameObject::update()
{
	src.x = 0;
	src.y = 0;
	src.w = size.w;
    src.h = size.h;
}
