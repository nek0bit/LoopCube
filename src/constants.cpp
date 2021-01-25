#include "constants.hpp"



// Texture stuff
namespace constants
{
	
	// TODO add enum for texture indexes
	const std::string versionStr = "v0.4-git";

	const std::string rootPath = DATA_LOCATION "/data/";
	const std::string path = rootPath+"img/";

	// Please add comment with index for code readability
	const std::vector<TextureInfo> textureInfo = {
		{"grass.png", TEXTURE_GRASS, 16, 16},
		{"dirt.png", TEXTURE_DIRT, 16, 16},
		{"stone.png", TEXTURE_STONE, 16, 16},
        {"wood.png", TEXTURE_WOOD, 16, 16},
		{"player.png", TEXTURE_PLAYER, 170, 33},
		{"menu_solid.png", TEXTURE_MENU_SOLID, 16, 32},
		{"menu_solid_left.png", TEXTURE_MENU_SOLID_LEFT, 16, 32},
		{"menu_solid_right.png", TEXTURE_MENU_SOLID_RIGHT, 16, 32},
		{"title_1.png", TEXTURE_TITLE_1, 16, 16},
		{"title_2.png", TEXTURE_TITLE_2, 16, 16},
		{"hotbar_slot.png", TEXTURE_HOTBAR_SLOT, 16, 32},
		{"inventory_menu.png", TEXTURE_INVENTORY_MENU, 170, 90},
		{"leaf.png", TEXTURE_LEAF, 16, 16},
		{"bg_shine.png", TEXTURE_BG_SHINE, 150, 200},
		{"bg_cloud_loop.png", TEXTURE_BG_CLOUD_LOOP, 450, 100},
		{"bg_hills.png", TEXTURE_BG_HILLS, 450, 114},
		{"bg_hills_hq.png", TEXTURE_BG_HILLS_HQ, 900, 310},
		{"cave_bg.png", TEXTURE_CAVE_BG, 16, 16},
		{"font_bitmap.png", TEXTURE_FONT_BITMAP, 160, 60},
		{"font_bitmap_x2.png", TEXTURE_FONT_BITMAP_X2, 320, 120},
		{"sun.png", TEXTURE_SUN, 100, 100},
		{"moon.png", TEXTURE_MOON, 100, 100},
		{"box1.png", TEXTURE_BOX1, 40, 40},
        {"box2.png", TEXTURE_BOX2, 40, 40},
        {"box3.png", TEXTURE_BOX3, 40, 40},
        {"box4.png", TEXTURE_BOX4, 40, 40},
        {"box5.png", TEXTURE_BOX5, 40, 40},
        {"box6.png", TEXTURE_BOX6, 40, 40},
        {"box7.png", TEXTURE_BOX7, 40, 40},
        {"box8.png", TEXTURE_BOX8, 40, 40},
        {"box9.png", TEXTURE_BOX9, 40, 40}
	};

	const std::vector<BlockInfo> blockInfo = {
		BlockInfo{BLOCK_GRASS, "Grass", TEXTURE_GRASS, false},
		BlockInfo{BLOCK_DIRT, "Dirt", TEXTURE_DIRT, false},
		BlockInfo{BLOCK_STONE, "Stone", TEXTURE_STONE, false},
		BlockInfo{BLOCK_WOOD, "Wood", TEXTURE_WOOD, true},
		BlockInfo{BLOCK_LEAF, "Leaf", TEXTURE_LEAF, true},
	};

	const double blockW = 42;
	const double blockH = 42;

	const int blockImgSize = 16;
		
	const int chunkWidth = 8;
	const int chunkHeight = 8;

#if defined(__WIIU__) || defined(__SWITCH__)
	const LoadDistance loadDistance{12, 12};
#else
	const LoadDistance loadDistance{16, 16};
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
