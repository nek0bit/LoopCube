#ifndef ITEM_HPP
#define ITEM_HPP
#include <iostream>
#include <string>
#include <memory>

#include <SDL2/SDL.h>

#include "text.hpp"
#include "constants.hpp"
#include "blockinfo.hpp"
#include "texturehandler.hpp"

class Item {
public:
	Item() = default;
    Item(std::string id, TextureHandler &textures, SDL_Renderer *renderer);
    ~Item();

    void draw(int x, int y, int width, int height);
    void add_count();
    int get_count();
    BlockInfo get_block();
private:
	std::shared_ptr<Text> text;
    std::string id;
    TextureHandler* textures;
    BlockInfo block;
    int count;
    SDL_Renderer *renderer;
};

#endif // ITEM_HPP
