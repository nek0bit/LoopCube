#include "block.hpp"

Block::Block() {}

Block::Block(std::string id, TextureHandler &textures, int x, int y)
    : Game_Object{0, textures, (double)x, (double)y, constants::block_w, constants::block_h} {

    for (auto &i: constants::block_info) {
        if (i.get_id() == id) {
            this->texture_id = i.get_texture_id();
            this->blockinfo = &i;
        }
    }
}

double Block::get_x(Camera& camera) const {
    return obj.x*obj.w + (camera.get_x());
}

double Block::get_y(Camera& camera) const {
    return obj.y*obj.h + (camera.get_y());

}

const BlockInfo* Block::get_blockinfo() {
    return blockinfo;
}

void Block::update(Camera& camera) {
    src.h = obj.h;
    src.w = obj.w;
    src.x = 0;
    src.y = 0;

    dest.h = src.h;
    dest.w = src.w;
    dest.x = get_x(camera);
    dest.y = get_y(camera);
}

void Block::render(SDL_Renderer* renderer) {
    // Disabled until blocks get sorted on placement
    //render_shadow(); // Note: When settings menu added, add option to disable shadow, it can be resource hungry
    SDL_RenderCopy(renderer, textures->get_texture(texture_id), &src, &dest);
}

void Block::render_shadow(SDL_Renderer* renderer) {
    SDL_Rect shadow;
    shadow.x = dest.x + 10;
    shadow.y = dest.y + 10;
    shadow.w = obj.w;
    shadow.h = obj.h;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 40);
    SDL_RenderFillRect(renderer, &shadow);
}
