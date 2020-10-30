#ifndef TEXT_HPP
#define TEXT_HPP
#include <iostream>
#include <string>
#include <array>

#include "backendincludes.hpp"

#include "rect.hpp"

class Text {
public:
	Text(std::string text = "");
	~Text();

	void render(GraphicsWrapper* renderer, int x, int y, int size = 10, bool high_quality = false);
	void operator=(const std::string& value);
	void set_text(std::string& text);

	int get_width();
	int get_height();
private:
	std::vector<int> char_width;
	std::string text;

	int text_width;
	int text_height;
};

#endif // TEXT_HPP
