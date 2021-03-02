#pragma once
#include <iostream>
#include <string>
#include <cstdint>
#include <vector>
#include <glm/glm.hpp>

#include "graphics.hpp"
#include "generic.hpp"
#include "gameobj.hpp"
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
    void renderShadow(const Graphics& graphics, const Camera& camera) const;
#endif

    void setBlockId(const uint32_t id);

    std::vector<unsigned char> serialize() const;
    void deserialize(const std::vector<unsigned char>& value);

	const BlockInfo* blockinfo;
    
    unsigned int typeX;
};

