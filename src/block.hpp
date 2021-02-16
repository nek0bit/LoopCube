#pragma once
#include <iostream>
#include <string>
#include <cstdint>
#include <array>
#include "gameobj.hpp"
#include "vector.hpp"
#include "constants.hpp"
#include "blockinfo.hpp"

struct Block: public GameObject
{
	Block();
	~Block();
	Block(int id, int x, int y, unsigned int typeX = 0);

    void update();
#ifndef __HEADLESS
    void updateSrc();
    void renderShadow(SDL_Renderer* renderer, Camera& camera) const;
#endif

    std::array<unsigned char, 10> serialize() const;
    void deserialize(const std::array<unsigned char, 10>& value);

	const BlockInfo* blockinfo;
    
    unsigned int typeX;
};

