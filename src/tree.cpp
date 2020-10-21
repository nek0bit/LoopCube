#include "tree.hpp"

Tree::Tree(int x, int y) : Structure{x, y} {
	int height = 9;
	for (int i = 0; i < height; ++i) {
		data.push_back({0, i*-1, BLOCK_WOOD});
	}
	// top
	int top_height = 4;
	int top_width = 2;
	for (int j = 0; j < top_height; ++j) {
		for (int k = top_width*-1; k <= top_width; ++k) {
			data.push_back({k, (height+j)*-1,BLOCK_LEAF});
		}
	}
	// top2
	int top2_height = 2;
	int top2_width = 1;
	for (int l = 0; l < top2_height; ++l) {
		for (int m = top2_width*-1; m <= top2_width; ++m) {
			data.push_back({m, (height+top_height+l)*-1, BLOCK_LEAF});
		}
	}
}

Tree::~Tree() {}
