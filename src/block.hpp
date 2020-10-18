#ifndef BLOCK_HPP
#define BLOCK_HPP
#include "gameobj.hpp"
#include "constants.hpp"
#include "blockinfo.hpp"

class Block: public Game_Object {

public:
    Block();
	~Block();
    Block(int id, TextureHandler &textures, int x, int y);

    void update(Camera& camera);
    void render(SDL_Renderer* renderer);
    void render_shadow(SDL_Renderer* renderer);
    const BlockInfo* get_blockinfo();
	double get_default_x() const override;
	double get_default_y() const override;
    double get_x(Camera& camera) const override;
    double get_y(Camera& camera) const override;
private:
    const BlockInfo *blockinfo;
	SDL_Rect shadow;
};


#endif // BLOCK_HPP
