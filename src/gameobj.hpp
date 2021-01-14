#pragma once

#include <string>

#include <SDL2/SDL.h>

#include "texturehandler.hpp"
#include "constants.hpp"
#include "position.hpp"
#include "camera.hpp"
#include "collisioninfo.hpp"

struct GameObject
{
	GameObject() = default;
	GameObject(int textureId, double x, double y, double w, double h);
	virtual ~GameObject() = default;

	virtual void update();
    virtual void render(SDL_Renderer* renderer, TextureHandler& textures, Camera& camera);

	virtual bool shouldCull(Camera& camera);

	// Return position
	virtual double getX(Camera& camera) const;
	virtual double getY(Camera& camera) const;

	CollisionInfo isColliding(const GameObject &obj2);

    Position obj;
protected:
	int textureId;
    SDL_Rect src;
};
