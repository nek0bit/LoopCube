#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP
#include <vector>
#include <utility>
#include <string>
#include "blockinfo.hpp"
#include "config.hpp"
#include <SDL2/SDL_ttf.h>

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
    extern const std::string path;

    // Please add comment with index for code readability
    extern const std::vector<const char*> texture_ids;

    extern const std::vector<BlockInfo> block_info;

    extern TTF_Font* button_font;
    extern TTF_Font* item_font;
	extern TTF_Font* header_font;
	extern TTF_Font* paragraph_font;
	extern TTF_Font* option_font;

    extern const double block_w;
    extern const double block_h;
    
    extern const int chunk_width;
	
    extern const int load_viewport;
    extern const int load_distance;

	extern const std::string header;
	extern const std::vector<std::string> content;

	extern Config config;
}

#endif // CONSTANTS
