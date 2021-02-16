#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include <atomic>
#include <cstdint>

#include "chunkgen.hpp"
#include "chunkgroup.hpp"
#include "timer.hpp"
#include "player.hpp"

struct GameServer
{
    GameServer();
    ~GameServer();

    void update(Timer& timer);
    std::vector<unsigned char> checkChunkAt(const long int x, const long int y);

    std::mutex chunkLock;
private:
    std::shared_ptr<ChunkGen> gen;
    ChunkGroup chunks;
};
