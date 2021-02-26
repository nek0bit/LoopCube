#pragma once

#include <vector>
#include <utility>
#include <string>

#ifndef __HEADLESS
#include "fonthandler.hpp"
#endif

#include "textureinfo.hpp"
#include "blockinfo.hpp"
#include "config.hpp"

enum block_enum
{
	BLOCK_MOON_BLOCK,
	BLOCK_WOOD
};

enum texture_enum
{
    TEXTURE_MOON_BLOCK,
    TEXTURE_WOOD,
    TEXTURE_PLAYER,
    TEXTURE_MENU_SOLID,
    TEXTURE_MENU_SOLID_LEFT,
    TEXTURE_MENU_SOLID_RIGHT,
    TEXTURE_HOTBAR_SLOT,
    TEXTURE_INVENTORY_MENU,
    TEXTURE_BG_SHINE,
    TEXTURE_BG_CLOUD_LOOP,
    TEXTURE_BG_HILLS,
    TEXTURE_BG_HILLS_HQ,
    TEXTURE_CAVE_BG,
    TEXTURE_SUN,
    TEXTURE_MOON,
    TEXTURE_BOX1,
    TEXTURE_BOX2,
    TEXTURE_BOX3,
    TEXTURE_BOX4,
    TEXTURE_BOX5,
    TEXTURE_BOX6,
    TEXTURE_BOX7,
    TEXTURE_BOX8,
    TEXTURE_BOX9
};

enum config_enum
{
	CONFIG_SHOW_SHADOWS,
	CONFIG_SHOW_PARTICLES,
	CONFIG_LOAD_DISTANCE,
	CONFIG_SHOW_CHUNK_DEBUG,
};

struct LoadDistance
{
    unsigned int x;
    unsigned int y;
};

namespace constants
{
	// Functions
	
	// Variables
	extern const std::string versionStr;

	extern const std::string rootPath;
	extern const std::string path;
    extern const std::string shaderPath;

	// Please add comment with index for code readability
	extern const std::vector<TextureInfo> textureInfo;

	extern const std::vector<BlockInfo> blockInfo;

	extern const int blockW;
	extern const int blockH;

	extern const int blockImgSize;
	
	extern const int chunkWidth;
	extern const int chunkHeight;
    
	extern LoadDistance loadDistance;

	extern const std::string header;
	extern const std::vector<std::string> content;

    // Public stuff
#ifndef __HEADLESS
    extern Config config;
    extern FontHandler fontHandler;
#endif
}
