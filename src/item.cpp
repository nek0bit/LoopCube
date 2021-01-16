#include "item.hpp"

Item::Item() : enabled{false} {}

Item::Item(SDL_Renderer* renderer, int id)
    : enabled{true},
      count{}
{
	for (auto &i: constants::blockInfo)
    {
		if (i.id == id)
        {
			block = i;
		}
	}

    SDL_Color t_Color{255, 255, 255, 255};
    text = std::make_shared<Text>(Text{renderer, "", t_Color, constants::fontHandler.getFont(0)});
    
}

void Item::addCount(int amount)
{
	count += amount;
    
	if (count == -1) {
		enabled = false;
	}

    text->setText(count == 0 ? "" : std::to_string(count+1));

}

void Item::render(SDL_Renderer* renderer, TextureHandler& textures,
                  int x, int y, int width = 35, int height = 35)
{
	int offset_y = -10;
	SDL_Rect src{0, 0, constants::blockImgSize, constants::blockImgSize};
    SDL_Rect block{x, y, width, height};
    SDL_RenderCopy(renderer, textures.getTexture(this->block.textureId), &src, &block);
    
	text->draw(x, y + height + offset_y);
}
