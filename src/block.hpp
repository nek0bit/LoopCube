#pragma once
#include <SDL2/SDL.h>

#include "gameobj.hpp"

#include "constants.hpp"
#include "blockinfo.hpp"

class Block: public GameObject {

public:
	Block();
	~Block();
	Block(int id, int x, int y);

	void update();
    void renderShadow(SDL_Renderer* renderer, Camera& camera) const;

	const BlockInfo* blockinfo;
};

