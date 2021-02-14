#pragma once
#include <iostream>
#include <string>
#include <vector>
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
};
