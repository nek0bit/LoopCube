#pragma once

#include <vector>
#include <utility>
#include <string>

#include "blockinfo.hpp"
#include "config.hpp"
#include "fonthandler.hpp"

enum block_enum
{
	BLOCK_GRASS,
	BLOCK_DIRT,
	BLOCK_STONE,
	BLOCK_WOOD,
	BLOCK_LEAF
};

enum config_enum
{
	CONFIG_SHOW_SHADOWS,
	CONFIG_SHOW_PARTICLES,
	CONFIG_LOAD_DISTANCE,
	CONFIG_SHOW_CHUNK_DEBUG,
};


namespace constants
{
	// Functions
	
	// Variables
	extern const std::string versionStr;

	extern const std::string rootPath;
	extern const std::string path;

	// Please add comment with index for code readability
	extern const std::vector<const char*> textureIds;

	extern const std::vector<BlockInfo> blockInfo;

	extern const double blockW;
	extern const double blockH;

	extern const int blockImgSize;
	
	extern const int chunkWidth;
	extern const int chunkHeight;

	extern const int chunkSplitCount;
	extern const int chunkSplitHeight;
	
	extern const int loadViewport;
	extern const int loadDistance;

	extern const std::string header;
	extern const std::vector<std::string> content;

    // Public stuff
	extern Config config;
    extern FontHandler fontHandler;
}
