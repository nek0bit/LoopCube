#ifndef TEXT_BITMAP_HPP
#define TEXT_BITMAP_HPP
#include <iostream>
#include <string>

#include "backendincludes.hpp"

#include "text.hpp"

class Text_Bitmap: public Text {
public:
	Text_Bitmap(std::string text = "");
	~Text_Bitmap();

	void render(GraphicsWrapper* renderer, int x, int y, int size = 10) override;
private:
	std::vector<int> char_width;
};

#endif
