#pragma once

#include <string>

#include <SDL2/SDL.h>

#include "texturehandler.hpp"
#include "constants.hpp"
#include "vector.hpp"
#include "size.hpp"
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
	virtual Vec2 getPos(Camera& camera) const;

	CollisionInfo isColliding(const GameObject &obj2);

    Vec2 position;
    Size size;
protected:
	int textureId;
    SDL_Rect src;
};
