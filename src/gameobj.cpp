#include "gameobj.hpp"

GameObject::GameObject() {}

GameObject::GameObject(int texture_id, double x, double y, double w, double h) {
	obj.x = x;
	obj.y = y;
	obj.w = w;
	obj.h = h;

	this->texture_id = texture_id;
}

GameObject::~GameObject()
{}

double GameObject::getX(Camera& camera) const
{
	return obj.x + camera.get_x();
}

double GameObject::getY(Camera& camera) const
{
	return obj.y + camera.get_y();
}

// Used for rendering culling
bool GameObject::shouldCull(Camera& camera)
{
    return (getX(camera) + obj.w < 0 || getY(camera) + obj.h < 0) // Upper-left culling
        || (getX(camera) - obj.w > camera.get_width() || getY(camera) - obj.h > camera.get_height()); // Bottom-right culling
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
    SDL_RenderCopy(renderer, textures.get_texture(texture_id), &src, &dest);
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
		const double top_calc = (r1.y+r1.h) - r2.y;
		const double bottom_calc = (r2.y+r2.h) - r1.y;
		const double left_calc = (r1.x+r1.w) - r2.x;
		const double right_calc = (r2.x+r2.h) - r1.x;

		CollisionInfo info{-1, -1, -1, -1};
		// top collision
		if (top_calc < bottom_calc) {
			info.top = top_calc;
		}

		// bottom collision
		if (top_calc > bottom_calc) {
			info.bottom = bottom_calc;
		}

		// left collision
		if (left_calc < right_calc) {
			info.left = left_calc;
		}

		// right collision
		if (left_calc > right_calc) {
			info.right = right_calc;
		}
		return info;
	}
	return CollisionInfo{};
}
