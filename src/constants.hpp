#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP
#include <vector>
#include <utility>
#include <string>

#include "blockinfo.hpp"
#include "config.hpp"

enum block_enum {
	BLOCK_GRASS,
	BLOCK_DIRT,
	BLOCK_STONE,
	BLOCK_WOOD,
	BLOCK_LEAF
};

enum config_enum {
	CONFIG_SHOW_SHADOWS,
	CONFIG_SHOW_PARTICLES,
	CONFIG_LOAD_DISTANCE,
	CONFIG_SHOW_CHUNK_DEBUG,
};

namespace constants {
	extern const std::string version_str;

	extern const std::string root_path;
	extern const std::string path;

	// Please add comment with index for code readability
	extern const std::vector<const char*> texture_ids;

	extern const std::vector<BlockInfo> block_info;

	extern const double block_w;
	extern const double block_h;
	
	extern const int chunk_width;
	extern const int chunk_height;

	extern const int chunk_split_count;
	extern const int chunk_split_height;
	
	extern const int load_viewport;
	extern const int load_distance;

	extern const std::string header;
	extern const std::vector<std::string> content;

	extern Config config;
}

#endif // CONSTANTS
