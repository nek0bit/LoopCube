#include "constants.hpp"

#ifndef DATA_LOCATION
#define DATA_LOCATION
#endif

// Texture stuff
namespace constants
{
	const std::string rootPath = ;
	const std::string path = rootPath + "img/";
    const std::string shaderPath = rootPath + "shaders/";

	// Please add comment with index for code readability
	const std::vector<TextureInfo> textureInfo = {
		{"moon_block.png", TEXTURE_MOON_BLOCK},
        {"wood.png", TEXTURE_WOOD},
		{"player.png", TEXTURE_PLAYER},
		{"menu_solid.png", TEXTURE_MENU_SOLID},
		{"menu_solid_left.png", TEXTURE_MENU_SOLID_LEFT},
		{"menu_solid_right.png", TEXTURE_MENU_SOLID_RIGHT},
		{"hotbar_slot.png", TEXTURE_HOTBAR_SLOT},
		{"inventory_menu.png", TEXTURE_INVENTORY_MENU},
		{"bg_shine.png", TEXTURE_BG_SHINE},
		{"bg_cloud_loop.png", TEXTURE_BG_CLOUD_LOOP},
		{"bg_hills.png", TEXTURE_BG_HILLS},
		{"bg_hills_hq.png", TEXTURE_BG_HILLS_HQ},
		{"cave_bg.png", TEXTURE_CAVE_BG},
		{"sun.png", TEXTURE_SUN},
		{"moon.png", TEXTURE_MOON},
		{"box1.png", TEXTURE_BOX1},
        {"box2.png", TEXTURE_BOX2},
        {"box3.png", TEXTURE_BOX3},
        {"box4.png", TEXTURE_BOX4},
        {"box5.png", TEXTURE_BOX5},
        {"box6.png", TEXTURE_BOX6},
        {"box7.png", TEXTURE_BOX7},
        {"box8.png", TEXTURE_BOX8},
        {"box9.png", TEXTURE_BOX9}
	};

	const std::vector<BlockInfo> blockInfo = {
		BlockInfo{BLOCK_MOON_BLOCK, "Moon Block", TEXTURE_MOON_BLOCK, false},
		BlockInfo{BLOCK_WOOD, "Wood", TEXTURE_WOOD, true},
	};

	const int blockW = 42;
	const int blockH = 42;

	const int blockImgSize = 16;
		
	const int chunkWidth = 8;
	const int chunkHeight = 8;

#if defined(__WIIU__) || defined(__SWITCH__)
	LoadDistance loadDistance{12, 12};
#else
	LoadDistance loadDistance{16, 16};
#endif

	const std::string header = "LoopCube";
	const std::vector<std::string> content = {
		"Welcome to LoopCube!\n",
		"Current Version: " + versionStr + "\n",
        "\"Stable\" Multiplayer Edition\n"
	};

#ifndef __HEADLESS
    Config config{};
    FontHandler fontHandler{};
#endif
}
