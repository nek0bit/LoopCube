#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <iostream>
#include "chunkgroup.hpp"
#include "chunk.hpp"
#include "block.hpp"
#include "gameobj.hpp"

class Player: public Game_Object {
public:
    Player() = default;
    Player(TextureHandler &textures);
    ~Player();

    void update(Camera& camera, Chunk_Group &chunks);
    bool check_block_collision(Camera& camera, Chunk_Group &chunks);

    // Movement
    void jump(Camera& camera, Chunk_Group &chunks);
    void direct_player(Camera& camera, int direction, Chunk_Group &chunks);

    double get_vel_x() const;
    double get_vel_y() const;
private:
    double vel_x;
    double vel_y;
    double vel_x_speed;
    int last_pos;
};

#endif // PLAYER_HPP
