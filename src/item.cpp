#include "item.hpp"

Item::Item() : enabled{false} {}

Item::Item(int id) : enabled{true}, count{} {
	for (auto &i: constants::block_info) {
		if (i.get_id() == id) {
			block = i;
		}
	}

	text = Text{};
}

Item::~Item() {

}

int Item::get_count() {
	return count;
}

BlockInfo Item::get_block() {
	return block;
}

void Item::add_count(int amount) {
	count += amount;
	if (count == -1) {
		enabled = false;
	}

	text = count == 0 ? "" : std::to_string(count+1);
}

void Item::render(SDL_Renderer* renderer, int x, int y, int width = 35, int height = 35) {
	int offset_y = -10;
	SDL_Rect src{0, 0, constants::block_img_size, constants::block_img_size};
    SDL_Rect block{x, y, width, height};
    SDL_RenderCopy(renderer, textures->get_texture(this->block.get_texture_id()), &src, &block);
    
	text.render(renderer, x, y+height+offset_y);
}
