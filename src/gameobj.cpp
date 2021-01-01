#include "gameobj.hpp"

GameObject::GameObject() {}

GameObject::GameObject(int texture_id, double x, double y, double w, double h) {
	obj.x = x;
	obj.y = y;
	obj.w = w;
	obj.h = h;

	this->texture_id = texture_id;
}

GameObject::~GameObject() {
}

double GameObject::get_default_x() const {
	return obj.x;
}

double GameObject::get_default_y() const {
	return obj.y;
}

double GameObject::get_x(Camera& camera) const {
	return obj.x + (camera.get_x());
}

double GameObject::get_y(Camera& camera) const {
	return obj.y + (camera.get_y());
}

double GameObject::get_width() const {
	return obj.w;
}

double GameObject::get_height() const {
	return obj.h;
}

// Used for rendering optimizations
bool GameObject::out_of_view(Camera& camera) {
	if (get_x(camera)+(obj.w*2) < 0 || get_y(camera)+(obj.h*2) < 0) {
		return true;
	}
	if (get_x(camera)-obj.w > camera.get_width() || get_y(camera)-obj.h > camera.get_height()) {
		return true;
	}
	return false;
}

void GameObject::update() {
	src.h = get_height();
	src.w = get_width();
	src.x = 0;
	src.y = 0;
}

void GameObject::render(SDL_Renderer* renderer, TextureHandler* textures, Camera& camera) {
	SDL_Rect dest{static_cast<int>(get_x(camera)), static_cast<int>(get_y(camera)), static_cast<int>(obj.w), static_cast<int>(obj.h)};
    SDL_RenderCopy(renderer, textures->get_texture(texture_id), &src, &dest);
}

const Position& GameObject::get_obj() const {
	return obj;
}

CollisionInfo GameObject::is_colliding(const GameObject &obj2) {
	// We are going to get the prefered positions instead of using their object
	// For example, the block class uses tiles, so when get_x is called, it returns the x in its obj and multiplies it by the width
	Position r1{get_default_x(), get_default_y(), get_width(), get_height()},
		r2{obj2.get_default_x(), obj2.get_default_y(), obj2.get_width(), obj2.get_height()};

	
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
