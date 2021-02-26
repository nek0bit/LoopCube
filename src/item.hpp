#pragma once
#include <iostream>
#include <string>
#include <memory>

#include "graphics.hpp"
#include "text.hpp"
#include "constants.hpp"
#include "blockinfo.hpp"
#include "texturehandler.hpp"

struct Item {
	Item();
	Item(const Graphics& graphics, int id);
	~Item() = default;

	void render(const Graphics& graphics, TextureHandler& textures, int x, int y, int width, int height) const;
	void addCount(int amount = 1);
    int count;
    BlockInfo block;
	bool enabled;
private:
    std::shared_ptr<Text> text;
	std::string id;
};
