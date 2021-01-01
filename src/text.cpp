#include "text.hpp"

Text::Text(std::string text) {
    set_text(text);
}

Text::~Text() {}

void Text::set_text(std::string& text) {
	this->text = text;
}

void Text::operator=(const std::string& value) {
    this->text = value;
}

void Text::render(GraphicsWrapper* renderer, int x, int y, int size) {}
