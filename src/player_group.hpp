#pragma once
#include <iostream>
#include <string>
#include <cstdint>
#include <memory>
#include <unordered_map>

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
    void renderPlayers(SDL_Renderer* renderer, TextureHandler& textures, Camera& camera);

    void updateSpecificPlayer(const int id, const int x, const int y);
    void addPlayer(const int id);
private:
    std::unordered_map<int, Player> players;
    
};
