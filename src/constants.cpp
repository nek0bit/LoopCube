#include "constants.hpp"

// Texture stuff
namespace constants {
	
	// TODO add enum for texture indexes
	const std::string version_str = "v0.3-git";

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
		"font_bitmap", // 18
		"font_bitmap_x2", // 19
		"sun", // 20
		"moon", // 21
		"box1", // 22
		"box2", // 23
		"box3", // 24
		"box4", // 25
		"box5", // 26
		"box6", // 27
		"box7", // 28
		"box8", // 29
		"box9", // 30
	};

	const std::vector<BlockInfo> block_info = {
		BlockInfo{BLOCK_GRASS, "Grass", 0},
		BlockInfo{BLOCK_DIRT, "Dirt", 1},
		BlockInfo{BLOCK_STONE, "Stone", 2},
		BlockInfo{BLOCK_WOOD, "Wood", 3, true},
		BlockInfo{BLOCK_LEAF, "Leaf", 12},
	};

	const double block_w = 42;
	const double block_h = 42;

	const int block_img_size = 16;
	
	const int load_viewport = 16;
	
	const int chunk_width = 8;
	const int chunk_height = 800;

	// chunk_height/x = integer(y)
	const int chunk_split_count = 100; // x
	const int chunk_split_height = constants::chunk_height/constants::chunk_split_count; // y

#if defined(__WIIU__) || defined(__SWITCH__)
	const int load_distance = 8;
#else
	const int load_distance = 12;
#endif

	const std::string header = "LoopCube";
	const std::vector<std::string> content = {
		"Welcome to LoopCube!\n",
		"Current Version: " + version_str + "\n",
        "Happy new year edition\n",
		"Changelog:",
		"-Semi-Verticle chunks",
		"-Faster chunk rendering",
		"-Bigger chunks",
		"-Better chunk generation",
		"-Less memory usage",
        "-Codebase changes",
		"  And much more..."
	};

	Config config;
}
