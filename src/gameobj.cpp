#include "gameobj.hpp"

GameObject::GameObject(int textureId, double x, double y, double w, double h)
    : obj{x, y, w, h},
      textureId{textureId},
      src{0, 0, 0, 0}
{
}

double GameObject::getX(Camera& camera) const
{
	return obj.x + camera.x;
}

double GameObject::getY(Camera& camera) const
{
	return obj.y + camera.y;
}

// Used for rendering culling
bool GameObject::shouldCull(Camera& camera)
{
    return (getX(camera) + obj.w < 0 || getY(camera) + obj.h < 0) // Upper-left culling
        || (getX(camera) - obj.w > camera.getWidth() || getY(camera) - obj.h > camera.getHeight()); // Bottom-right culling
}

void GameObject::update()
{
	src.x = 0;
	src.y = 0;
	src.w = obj.w;
    src.h = obj.h;
}

void GameObject::render(SDL_Renderer* renderer, TextureHandler& textures, Camera& camera)
{
	SDL_Rect dest{static_cast<int>(getX(camera)),
        static_cast<int>(getY(camera)),
        static_cast<int>(obj.w),
        static_cast<int>(obj.h)};
    SDL_RenderCopy(renderer, textures.get_texture(textureId), &src, &dest);
}

CollisionInfo GameObject::isColliding(const GameObject &obj2)
{
    // Aliases
    const Position& r1 = obj,
        r2 = obj2.obj;
	
	if (r1.x < r2.x + r2.w &&
		r1.x + r1.w > r2.x &&
		r1.y < r2.y + r2.h &&
		r1.y + r1.h > r2.y) {
		const double topCalc = (r1.y+r1.h) - r2.y;
		const double bottomCalc = (r2.y+r2.h) - r1.y;
		const double leftCalc = (r1.x+r1.w) - r2.x;
		const double rightCalc = (r2.x+r2.h) - r1.x;

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
