#include "block.hpp"

Block::Block() {}

Block::Block(int id, int x, int y)
	: GameObject{0, static_cast<double>(x), static_cast<double>(y), constants::block_w, constants::block_h} {

	
	for (auto &i: constants::block_info) {
		if (i.get_id() == id) {
			this->texture_id = i.get_texture_id();
			this->blockinfo = &i;
		}
	}
}

Block::~Block() {}

double Block::get_default_x() const {
	return obj.x*constants::block_w;
}

double Block::get_default_y() const {
	return obj.y*constants::block_h;
}

double Block::get_x(Camera& camera) const {
	return obj.x*constants::block_w + (camera.get_x());
}

double Block::get_y(Camera& camera) const {
	return obj.y*constants::block_h + (camera.get_y());
}

const BlockInfo* Block::get_blockinfo() {
	return blockinfo;
}

void Block::update() {
	src.h = constants::block_img_size;
	src.w = constants::block_img_size;
	src.x = 0;
	src.y = 0;
}

void Block::render_shadow(SDL_Renderer* renderer, Camera& camera) const {
	Rect shadow{static_cast<int>(get_x(camera) + 10), static_cast<int>(get_y(camera) + 10),
		static_cast<int>(obj.w), static_cast<int>(obj.h)};
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 40);
    SDL_RenderFillRect(renderer, &shadow);
}
