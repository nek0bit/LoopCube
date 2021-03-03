#pragma once

#include <array>
#include <vector>
#include <utility>
#include <string>

#ifndef __HEADLESS
#include "fonthandler.hpp"
#endif

#include "textureinfo.hpp"
#include "blockinfo.hpp"
#include "config.hpp"

#ifndef DATA_LOCATION
#define DATA_LOCATION
#endif

#ifndef ROOT_PATH
#define ROOT_PATH DATA_LOCATION "/data/"
#endif

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
	// Variables
	constexpr char versionStr[] = "v0.5";
    constexpr char rootPath[] = ROOT_PATH;
    constexpr char path[] = ROOT_PATH "img/";
    constexpr char shaderPath[] = ROOT_PATH "shaders/";

	// Please add comment with index for code readability
    constexpr std::array<TextureInfo, 24> textureInfo = {
		TextureInfo{"moon_block.png", TEXTURE_MOON_BLOCK, 16, 16},
        TextureInfo{"wood.png", TEXTURE_WOOD, 16, 16},
		TextureInfo{"player.png", TEXTURE_PLAYER, 17, 33},
		TextureInfo{"menu_solid.png", TEXTURE_MENU_SOLID, 16, 16},
		TextureInfo{"menu_solid_left.png", TEXTURE_MENU_SOLID_LEFT, 16, 16},
		TextureInfo{"menu_solid_right.png", TEXTURE_MENU_SOLID_RIGHT, 16, 16},
		TextureInfo{"hotbar_slot.png", TEXTURE_HOTBAR_SLOT, 16, 32},
		TextureInfo{"inventory_menu.png", TEXTURE_INVENTORY_MENU, 170, 90},
		TextureInfo{"bg_shine.png", TEXTURE_BG_SHINE, 150, 200},
		TextureInfo{"bg_cloud_loop.png", TEXTURE_BG_CLOUD_LOOP, 450, 100},
		TextureInfo{"bg_hills.png", TEXTURE_BG_HILLS, 450, 114},
		TextureInfo{"bg_hills_hq.png", TEXTURE_BG_HILLS_HQ, 900, 310},
		TextureInfo{"cave_bg.png", TEXTURE_CAVE_BG, 16, 16},
		TextureInfo{"sun.png", TEXTURE_SUN, 100, 100},
		TextureInfo{"moon.png", TEXTURE_MOON, 100, 100},
		TextureInfo{"box1.png", TEXTURE_BOX1, 40, 40},
        TextureInfo{"box2.png", TEXTURE_BOX2, 40, 40},
        TextureInfo{"box3.png", TEXTURE_BOX3, 40, 40},
        TextureInfo{"box4.png", TEXTURE_BOX4, 40, 40},
        TextureInfo{"box5.png", TEXTURE_BOX5, 40, 40},
        TextureInfo{"box6.png", TEXTURE_BOX6, 40, 40},
        TextureInfo{"box7.png", TEXTURE_BOX7, 40, 40},
        TextureInfo{"box8.png", TEXTURE_BOX8, 40, 40},
        TextureInfo{"box9.png", TEXTURE_BOX9, 40, 40}
	};

    constexpr std::array<BlockInfo, 2> blockInfo = {
	    BlockInfo{BLOCK_MOON_BLOCK, "Moon Block", TEXTURE_MOON_BLOCK, false},
		BlockInfo{BLOCK_WOOD, "Wood", TEXTURE_WOOD, true}
    };

	constexpr int blockW = 42;
    constexpr int blockH = 42;

    // Deprecated
    constexpr int blockImgSize = 16;
    constexpr int chunkWidth = 8;
    constexpr int chunkHeight = 8;
	    
	extern LoadDistance loadDistance;

	extern const std::string header;
	extern const std::vector<std::string> content;

    // Public stuff
#ifndef __HEADLESS
    extern Config config;
    extern FontHandler fontHandler;
#endif
}
