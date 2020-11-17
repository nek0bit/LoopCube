#include "text_full.hpp"

Text* GENERATE_TEXT(std::string text) {
#ifdef TEXT_BITMAP
	return new Text_Bitmap(text);
#else
	return new Text_Bitmap(text);
#endif
}
