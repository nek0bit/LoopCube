#include "fonthandler.hpp"

FontHandler::FontHandler() : fonts{}
{}

FontHandler::~FontHandler()
{
    for (TTF_Font*& close: fonts)
    {
        TTF_CloseFont(close);
    }
}

size_t FontHandler::addFontByFilename(std::string filename, size_t size)
{
    TTF_Font* font = TTF_OpenFont(filename.c_str(), size);

    if (font == nullptr)
    {
        static bool segfMsg = false;
        if (!segfMsg)
        {
            std::cout << "]]] Failed to load (possibly multiple fonts)... Beware dragons and segfaults" << std::endl;
            segfMsg = true;
        }
        std::cerr << "[Error] Couldn't load " <<
            filename << ": " << TTF_GetError() << std::endl;
    }
    
    fonts.push_back(font);

    return fonts.size()-1; // Should return the index of new item
}

void FontHandler::addFontByFilename(std::string filename, std::vector<size_t> size)
{
    for (auto& i: size)
        addFontByFilename(filename, i);
}

TTF_Font* FontHandler::getFont(size_t index) noexcept
{
    if (index > fonts.size())
    {
        return nullptr;
    }
    
    return fonts[index];
}

TTF_Font* FontHandler::operator[](size_t index) noexcept
{
    return fonts[index];
}
