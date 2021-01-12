#include "constants.hpp"

// Texture stuff
namespace constants
{
	
	// TODO add enum for texture indexes
	const std::string versionStr = "v0.4-git";

	const std::string rootPath = DATA_LOCATION "/data/";
	const std::string path = rootPath+"img/";

	// Please add comment with index for code readability
	const std::vector<const char*> textureIds = {
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

	const std::vector<BlockInfo> blockInfo = {
		BlockInfo{BLOCK_GRASS, "Grass", 0, false},
		BlockInfo{BLOCK_DIRT, "Dirt", 1, false},
		BlockInfo{BLOCK_STONE, "Stone", 2, false},
		BlockInfo{BLOCK_WOOD, "Wood", 3, true},
		BlockInfo{BLOCK_LEAF, "Leaf", 12, true},
	};

	const double blockW = 42;
	const double blockH = 42;

	const int blockImgSize = 16;
	
	const int loadViewport = 16;
	
	const int chunkWidth = 8;
	const int chunkHeight = 800;

	// chunk_height/x = integer(y)
	const int chunkSplitCount = 100; // x
	const int chunkSplitHeight = constants::chunkHeight/constants::chunkSplitCount; // y

#if defined(__WIIU__) || defined(__SWITCH__)
	const int loadDistance = 8;
#else
	const int loadDistance = 12;
#endif

	const std::string header = "LoopCube";
	const std::vector<std::string> content = {
		"Welcome to LoopCube!\n",
		"Current Version: " + versionStr + "\n",
        "Very tested edition\n",
		"Changelog:",
		"- LOTS of code refactoring [?]",
        "- Polymorphic backgrounds [x]",
        "- DeltaTime [ ]",
        "- Camera zoom [ ]",
	};

	Config config{};
    FontHandler fontHandler{};
}
