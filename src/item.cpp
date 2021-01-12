#include "item.hpp"

Item::Item() : enabled{false} {}

Item::Item(SDL_Renderer* renderer, int id) : enabled{true}, count{} {
	for (auto &i: constants::block_info) {
		if (i.id == id) {
			block = i;
		}
	}

    SDL_Color t_Color{255, 255, 255, 255};
    text = std::make_shared<Text>(Text{renderer, "", t_Color, constants::fontHandler.getFont(0)});
    
}

Item::~Item() {}

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

    text->set_text(count == 0 ? "" : std::to_string(count+1));

}

void Item::render(SDL_Renderer* renderer, TextureHandler& textures, int x, int y, int width = 35, int height = 35) {
	int offset_y = -10;
	SDL_Rect src{0, 0, constants::block_img_size, constants::block_img_size};
    SDL_Rect block{x, y, width, height};
    SDL_RenderCopy(renderer, textures.get_texture(this->block.textureId), &src, &block);
    
	text->draw(x, y+height+offset_y);
}
