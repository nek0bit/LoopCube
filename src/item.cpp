#include "item.hpp"

Item::Item() : enabled{false} {}

Item::Item(int id, GraphicsWrapper* renderer) : enabled{true}, count{} {
	this->renderer = renderer;
	
	for (auto &i: constants::block_info) {
		if (i.get_id() == id) {
			block = i;
		}
	}

	//SDL_Color color{};
	//color.r = 255; color.g = 255; color.b = 255;
	//text = std::shared_ptr<Text>(new Text(renderer, "", color, constants::item_font));
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
	//text->set_text(count == 0 ? "" : std::to_string(count+1).c_str());
}

void Item::draw(int x, int y, int width = 35, int height = 35) {
	Rect src{0, 0, width, height};
	Rect block{x, y, width, height};
	renderer->render(src, block, this->block.get_texture_id());

	//text->draw(x, y+height);
}
