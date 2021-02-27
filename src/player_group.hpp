#pragma once
#include <iostream>
#include <string>
#include <cstdint>
#include <memory>
#include <unordered_map>

#include "graphics.hpp"
#include "timer.hpp"
#include "chunkgroup.hpp"
#include "entity.hpp"
#include "player.hpp"

struct PlayerGroup
{
    PlayerGroup();
    ~PlayerGroup();

    void updatePlayers(ChunkGroup &chunks, Timer &timer,
                       std::vector<std::shared_ptr<Entity>>& entities);
    void renderPlayers(const Graphics& renderer, TextureHandler& textures, const Camera& camera) const;

    void updateSpecificPlayer(const int id, const int x, const int y);
    void addPlayer(const int id);
private:
    std::unordered_map<int, Player> players;
    
};
