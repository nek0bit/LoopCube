#include "text.hpp"

Text::Text(std::string text) {
	char_width = {
	    5, // SPACE
		3, // !
		5, // "
		7, // #
		7, // $
		9, // %
		8, // &
		3, // '
		5, // (
		5, // )
		7, // *
		9, // +
		3, // ,
		7, // -
		2, // .
		6, // F-SLASH
		8, // 0
		7, // 1
		6, // 2
		6, // 3
		6, // 4
		6, // 5
		6, // 6
		6, // 7
		6, // 8
		6, // 9
		2, // :
		2, // ;
		6, // <
		8, // =
		6, // >
		6, // ?
		8, // @
		7, // A
		6, // B
		6, // C
		6, // D
		6, // E
		6, // F
		6, // G
		6, // H
		5, // I
		6, // J
		6, // K
		6, // L
		7, // M
		7, // N
		6, // O
		6, // P
		6, // Q
		6, // R
		6, // S
		5, // T
		6, // U
		7, // V
		7, // W
		7, // X
		5, // Y
		5, // Z
		5, // [
		6, // B-SLASH
		5, // ]
		5, // ^
		8, // _
		4, // BACKTICK
		6, // a
		6, // b
		6, // c
		6, // d
		5, // e
		5, // f
		5, // g
		6, // h
		3, // i
		5, // j
		5, // k
		3, // l
		7, // m
		6, // n
		6, // o
		6, // p
		6, // q
		5, // r
		5, // s
		5, // t
		5, // u
		6, // v
		7, // w
		5, // x
		5, // y
		6, // z
		5, // CURLY-LEFT
		3, // |
		5, // CURLY-RIGHT
		6, // ~	
	};
    set_text(text);
}

Text::~Text() {}

void Text::set_text(std::string& text) {
	this->text = text;
}

void Text::operator=(const std::string& value) {
    this->text = value;
}

void Text::render(GraphicsWrapper* renderer, int x, int y, int size, bool high_quality) {
	const int height = high_quality ? 20 : 10;
	
	int ascii_start = 32;
	int columns = 16;
	int char_gap = 0;
	int scale_size = size-height;

	int char_multiplier = height/10;

	int render_x = 0;
	int render_y = 0;
	for(char& c: this->text) {
		unsigned int index = static_cast<int>(c) - ascii_start;
		int row = index/16;
		int col_mod = index % columns;

		if (index > char_width.size()) index = 0;
	    switch(c) {
		case '\n':
			render_y += size;
			render_x = 0;
			break;
		default:
			Rect src{col_mod*height, row*height, (char_width[index]*char_multiplier), height};
			Rect dest{x+render_x, y+render_y, (char_width[index]*char_multiplier)+char_gap+scale_size, size};

			render_x += (char_width[index]*char_multiplier)+scale_size;
			renderer->render(src, dest, high_quality ? 19 : 18);
			break;
		}
	}
	text_width = render_x+scale_size;
	text_height = render_y+size;
}

int Text::get_width() {
	return text_width;
}

int Text::get_height() {
	return text_height;
}
