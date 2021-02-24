#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <cstdint>
#include <memory>
#include <unistd.h>
#include <functional>
#include <vector>

#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "command_constants.hpp"
#include "generic.hpp"

namespace Api
{
    void sendPlayerPos(const int fd, const int x, const int y);
    void sendRecvChunk(const int fd, const long chunkX, const long chunkY);
    void sendPlaceBlock(const int fd, const uint32_t id,
                        const int64_t chunkX, const int64_t chunkY,
                        const uint16_t x, const uint16_t y);
    void sendDestroyBlock(const int fd,
                          const int64_t chunkX, const int64_t chunkY,
                          const uint16_t x, const uint16_t y);
}
