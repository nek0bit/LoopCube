#pragma once

#include <string>

#include <SDL2/SDL.h>

#include "texturehandler.hpp"
#include "constants.hpp"
#include "position.hpp"
#include "camera.hpp"
#include "collisioninfo.hpp"

class GameObject {

public:
	GameObject();
	GameObject(int texture_id, double x, double y, double w, double h);
	virtual ~GameObject();

	virtual void update();
    virtual void render(SDL_Renderer* renderer, TextureHandler* textures, Camera& camera);

	virtual bool out_of_view(Camera& camera);

	// Return position
	virtual double get_default_x() const;
	virtual double get_default_y() const;
	virtual double get_x(Camera& camera) const;
	virtual double get_y(Camera& camera) const;
	virtual double get_width() const;
	virtual double get_height() const;

	virtual const Position& get_obj() const;

	CollisionInfo is_colliding(const GameObject &obj2);
protected:
	int texture_id;

	Position obj;
    SDL_Rect src;
};
