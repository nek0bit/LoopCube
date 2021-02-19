#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <cstdint>
#include <memory>
#include <unistd.h>

#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "server.hpp"

namespace Api
{
    void sendEcho(const int fd, const std::string data);
    void sendPlayerPos(const int fd, const double x, const double y);
    void sendRecvChunk(const int fd, const long x, const long y);
}
