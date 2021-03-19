#pragma once

#include <iostream>
#include <vector>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

struct FontHandler
{
    FontHandler();
    ~FontHandler();

    size_t addFontByFilename(std::string filename, size_t size);
    void addFontByFilename(std::string filename, std::vector<size_t> size);

    TTF_Font* getFont(size_t index) noexcept;
    TTF_Font* operator[](size_t index) noexcept;
private:
    std::vector<TTF_Font*> fonts;
};
