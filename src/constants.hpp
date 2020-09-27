#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP
#include <vector>
#include <utility>
#include <string>
#include "blockinfo.hpp"
#include <SDL2/SDL_ttf.h>

namespace constants {
    extern const std::string path;

    // Please add comment with index for code readability
    extern const std::vector<const char*> texture_ids;

    extern const std::vector<BlockInfo> block_info;

    extern TTF_Font* button_font;
    extern TTF_Font* item_font;

    extern const double block_w;
    extern const double block_h;
    
    extern const int chunk_width;
    
    extern const int load_viewport;

    extern const int load_distance;
}

#endif // CONSTANTS