#pragma once
#include <string>
#include <memory>

#include "graphics.hpp"
#include "text.hpp"
#include "constants.hpp"
#include "texturehandler.hpp"
#include "blockinfo.hpp"

struct Item {
	Item();
	Item(const Graphics& graphics, int id);
	~Item() = default;

	void render(const Graphics& graphics, int x, int y, int width, int height) const;
	void addCount(int amount = 1);
    const BlockInfo& getBlockInfo() const;

    int count;
	bool enabled;
    int id;
private:
    std::shared_ptr<Text> text;
};
