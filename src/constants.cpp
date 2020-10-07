#include "constants.hpp"

// Texture stuff
namespace constants {
	// I thought it was funny to reference developer "git" versions
	// by putting a "x0" at the end of them :)
	const std::string version_str = "v0.2x0";
	
    const std::string path = "data/img/";

    // Please add comment with index for code readability
    const std::vector<const char*> texture_ids = {
        "grass", // 0
        "dirt", // 1
        "stone", // 2
        "wood", // 3
        "player", // 4
        "menu_solid", // 5
        "menu_rounded_left", // 6
        "menu_rounded_right", // 7
        "title_1", // 8
        "title_2", // 9
        "hotbar_slot", // 10
        "inventory_menu", // 11
        "leaf", // 12
		"bg_shine", // 13
    };

    const std::vector<BlockInfo> block_info = {
        BlockInfo{BLOCK_GRASS, "Grass", 0},
        BlockInfo{BLOCK_DIRT, "Dirt", 1},
        BlockInfo{BLOCK_STONE, "Stone", 2},
        BlockInfo{BLOCK_WOOD, "Wood", 3, true},
        BlockInfo{BLOCK_LEAF, "Leaf", 12},
    };

	// Assignment in game.cpp
    TTF_Font* button_font = nullptr;
    TTF_Font* item_font = nullptr;
	TTF_Font* header_font = nullptr;
	TTF_Font* paragraph_font = nullptr;
	TTF_Font* option_font = nullptr;

    const double block_w = 36;
    const double block_h = 36;
    
    const int load_viewport = 6;
    
    const int chunk_width = 8;

    #if defined(__WIIU__) || defined(__SWITCH__)
        const int load_distance = 8;
    #else
        const int load_distance = 12;
    #endif

	const std::string header = "LoopCube";
	const std::vector<std::string> content = {
		"Welcome to LoopCube!\n",
		"The current version is " + version_str + "\n",
		"[  --  Changelog: v0.1  --  ]",
		"- Wii U support",
		"- Mostly optimizations",
		"- The first version to be fair",
		"- Playable"
	};

	Config config;
}
