#pragma once
#include <iostream>
#include <string>
#include <cmath>
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
    void modifyBlock(const uint32_t id,
                     int64_t chunkX, int64_t chunkY,
                     uint64_t x, uint64_t y,
                     bool createOrDestroy);

    std::mutex chunkLock;
private:
    std::shared_ptr<ChunkGen> gen;
    ChunkGroup chunks;
};
