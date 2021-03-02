#include "item.hpp"

Item::Item() : enabled{false}, id{0} {}

Item::Item(const Graphics& graphics, int id)
    : count{},
      enabled{true},
      id{id}
{
    SDL_Color t_Color{255, 255, 255, 255};
    //text = std::make_shared<Text>(Text{renderer, "", t_Color, constants::fontHandler.getFont(0)});
    
}

void Item::addCount(int amount)
{
	count += amount;
    
	if (count == -1) {
		enabled = false;
	}

    //if (text != nullptr) text->setText(count == 0 ? "" : std::to_string(count+1));

}

const BlockInfo& Item::getBlockInfo() const
{
    return constants::blockInfo[id];
}

void Item::render(const Graphics& graphics, int x, int y, int width = 35, int height = 35) const
{
	constexpr int offset_y = -10;
	SDL_Rect src{4*constants::blockImgSize, 0, constants::blockImgSize, constants::blockImgSize};
    SDL_Rect block{x, y, width, height};
    //SDL_RenderCopy(renderer, textures.getTexture(this->block.textureId)->texture, &src, &block);
    
	//text->draw(graphics, x, y + height + offset_y);
}
