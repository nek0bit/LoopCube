#pragma once
#include <string>
#include <thread>
#include <vector>
#include <cstdint>
#include <poll.h>

struct ConnectionData
{
    std::string username;
    double playerX;
    double playerY;
};

// Struct for thread items
struct ServerThreadItem
{
    uint16_t id;
    std::thread thread;
    uint32_t count;
    std::vector<pollfd> connections; // Must be pollfd's or poll won't work
    std::vector<ConnectionData> connectionData; // Alligned to connections vector
};
