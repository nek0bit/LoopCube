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
	virtual ~Text();

	virtual void render(GraphicsWrapper* renderer, int x, int y, int size = 10);
	virtual void operator=(const std::string& value);
	virtual void set_text(std::string& text);

	int get_width();
    int get_height();
protected:
	std::vector<int> char_width;
	std::string text;

	int text_width;
	int text_height;
};

#endif // TEXT_HPP
