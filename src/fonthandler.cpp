#include "fonthandler.hpp"

FontHandler::FontHandler() : fonts{}
{}

FontHandler::~FontHandler()
{

}

size_t FontHandler::addFontByFilename(std::string filename, size_t size)
{
    TTF_Font* font = TTF_OpenFont(filename.c_str(), size);
    fonts.push_back(font);

    return fonts.size()-1; // Should return the index of new item
}

void FontHandler::addFontByFilename(std::string filename, std::vector<size_t> size)
{
    for (auto& i: size)
        addFontByFilename(filename, i);
}

TTF_Font* FontHandler::getFont(size_t index)
{
    if (index > fonts.size()-1)
        return nullptr;
    
    return fonts[index];
}
