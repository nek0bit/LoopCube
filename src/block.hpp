#pragma once

#include <SDL2/SDL.h>

#include "gameobj.hpp"

#include "vector.hpp"
#include "constants.hpp"
#include "blockinfo.hpp"

struct Block: public GameObject
{
	Block();
	~Block();
	Block(int id, int x, int y, unsigned int typeX = 0, unsigned int typeY = 0);

	void update();
    void updateSrc();
    void renderShadow(SDL_Renderer* renderer, Camera& camera) const;

	const BlockInfo* blockinfo;
    
    unsigned int typeX;
    unsigned int typeY;
};

