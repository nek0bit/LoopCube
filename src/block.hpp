#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "rect.hpp"
#include "color.hpp"
#include "gameobj.hpp"
#include "constants.hpp"
#include "blockinfo.hpp"

class Block: public GameObject {

public:
	Block();
	~Block();
	Block(int id, int x, int y);

	void update();
    void render_shadow(GraphicsWrapper* renderer, Camera& camera) const;
    const BlockInfo* get_blockinfo();
	double get_default_x() const override;
	double get_default_y() const override;
	inline double get_x(Camera& camera) const override;
	inline double get_y(Camera& camera) const override;
private:
	const BlockInfo *blockinfo;
};


#endif // BLOCK_HPP
