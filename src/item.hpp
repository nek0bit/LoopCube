#ifndef ITEM_HPP
#define ITEM_HPP
#include <iostream>
#include <string>
#include <memory>

#include "backendincludes.hpp"

#include "rect.hpp"
#include "text.hpp"
#include "constants.hpp"
#include "blockinfo.hpp"
#include "texturehandler.hpp"

class Item {
public:
	Item();
	Item(int id);
	~Item();

	void draw(GraphicsWrapper* renderer, int x, int y, int width, int height);
	void add_count(int amount = 1);
	int get_count();
	BlockInfo get_block();
	bool enabled;
private:
	Text text;
	std::string id;
	BlockInfo block;
	int count;
};

#endif // ITEM_HPP
