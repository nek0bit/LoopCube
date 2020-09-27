#include "constants.hpp"

// TODO This file is LESS ugly than it used to, but I keep using it, send help
// NOTE maybe move to texture.cpp?

// Texture stuff
namespace constants {
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
    };

    const std::vector<BlockInfo> block_info = {
        BlockInfo{"grass", "Grass", 0},
        BlockInfo{"dirt", "Dirt", 1},
        BlockInfo{"stone", "Stone", 2},
        BlockInfo{"wood", "Wood", 3, true},
        BlockInfo{"leaf", "Leaf", 12},
    };

    TTF_Font* button_font = nullptr;
    TTF_Font* item_font = nullptr;

    const double block_w = 36;
    const double block_h = 36;
    
    const int load_viewport = 6;
    
    const int chunk_width = 8;

    #if defined(__WIIU__) || defined(__SWITCH__)
        const int load_distance = 8;
    #else
        const int load_distance = 12;
    #endif
}