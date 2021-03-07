#pragma once
#include <string>

#ifndef __HEADLESS
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "graphics.hpp"
#include "texturehandler.hpp"
#include "camera.hpp"
#endif // __HEADLESS

#include "generic.hpp"
#include "constants.hpp"
#include "collisioninfo.hpp"

struct GameObject
{
	GameObject() = default;
	GameObject(int modelId, int textureId, const glm::vec3& position, const glm::vec2& size);
	virtual ~GameObject() = default;

    virtual void update();
    
#ifndef __HEADLESS
    virtual void render(const Graphics& graphics, const Camera& camera) const;

	virtual bool shouldCull(const Camera& camera) const;
#endif // __HEADLESS

	CollisionInfo isColliding(const GameObject &obj2) const;

    glm::vec3 position;
    glm::vec3 size;

    int modelId, textureId;
protected:
    SDL_Rect src; // Deprecated
};
