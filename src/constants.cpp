#include "constants.hpp"

// Texture stuff
namespace constants {
	// I thought it was funny to reference developer "git" versions
	// by putting a "x0" at the end of them :)
	const std::string version_str = "v0.3x0";

	const std::string root_path = DATA_LOCATION "/data";
	const std::string path = root_path+"/img/";

	// Please add comment with index for code readability
	const std::vector<const char*> texture_ids = {
		"grass", // 0
		"dirt", // 1
		"stone", // 2
		"wood", // 3
		"player", // 4
		"menu_solid", // 5
		"menu_solid_left", // 6
		"menu_solid_right", // 7
		"title_1", // 8
		"title_2", // 9
		"hotbar_slot", // 10
		"inventory_menu", // 11
		"leaf", // 12
		"bg_shine", // 13
		"bg_cloud_loop", // 14
		"bg_hills", // 15
		"bg_hills_hq", // 16
		"cave_bg", // 17
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

	const double block_w = 42;
	const double block_h = 42;
	
	const int load_viewport = 16;
	
	const int chunk_width = 8;
	const int chunk_height = 1024;

	// chunk_height/x = integer(y)
	const int chunk_split_count = 256; // x
	const int chunk_split_height = constants::chunk_height/constants::chunk_split_count; // y

#if defined(__WIIU__) || defined(__SWITCH__)
	const int load_distance = 8;
#else
	const int load_distance = 12;
#endif

	const std::string header = "LoopCube";
	const std::vector<std::string> content = {
		"Welcome to LoopCube!\n",
		"The current version is " + version_str + "\n",
		"<><><> Changelog: v0.2 <><><>",
		"- Beautiful Parallax Backgronds",
		"- Chunk size increased, performs",
		"	much better then before",  
		"- Some code restructuring",
		"- Switch Build",
		"- Particles",
		"- Menu progress",
		"- Config options",
		"- Inventory",
		"- Lots of bugs fixed",
	};

	Config config;
}
