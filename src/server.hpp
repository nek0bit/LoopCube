#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <thread>
#include <sstream>
#include <numeric>
#include <iterator>
#include <chrono>
#include <mutex>
#include <atomic>
#include <algorithm>
#include <cstring>
#include <cstdint>
#include <exception>
#include <poll.h>

#include "socketwrapper.hpp"
#include "gameserver.hpp"
#include "timer.hpp"

// Messages
constexpr const char* _COMMAND_SUCCESS = "ACK"; // This shouldn't be sent very often!
constexpr const char* _COMMAND_FAIL = "NACK";

constexpr const char* COMMAND_QUIT = "QUIT";
constexpr const char* COMMAND_ECHO = "ECHO";
constexpr const char* COMMAND_PLAYER_POS = "PPOS";
constexpr const char* COMMAND_GET_CHUNK = "GC";

struct Server;
struct ServerThreadItem;
struct ThreadData;
struct NetworkError;

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

// Error handling
enum NERROR_TYPE
{
    NRESOLUTION_ERROR = 1,
    NSOCKOPT_ERROR,
    NSOCKBIND_ERROR,
    NLISTEN_ERROR
};

struct NetworkError: public std::exception
{
    NetworkError(NERROR_TYPE type, const char* error)
        : std::exception(), type{type}, error{error} {};
    virtual const char* what() const throw() { return error; };
    NERROR_TYPE type;
private:
    const char* error;
};

// LOGGER (for server)
namespace ServLog
{
    bool warn(const std::string msg);
    bool error(const std::string msg);
    bool info(const std::string msg);
    bool log(const std::string msg);
}

// Server
struct Server
{
    Server(const uint32_t port, bool verbose = true);
    ~Server();

    void startServer(const size_t threadCount = 1); // Note: Blocking
    void serverThread(const size_t index) noexcept;
    void startGameThread() noexcept;
    
    std::atomic<bool> exit;
    int fd;
private:
    template <typename Iterator>
    std::string combineString(Iterator begin,
                              Iterator end,
                              const std::string delim = "")
    {
        std::stringstream str;
        for (std::vector<std::string>::iterator i = begin; i != end; ++i)
        {
            str << *i;
            if (i != end-1) str << delim;
        }
        return str.str();
    }
    void removeConnection(ServerThreadItem& item, const size_t index);
    void handleCommand(char* buffer, ServerThreadItem& item, const size_t index);
    ServerThreadItem& minThreadCount();
#ifndef __NOIPLOG__
    std::string getAddress(sockaddr* info);
#endif
    const uint32_t port;
    std::string address;
    std::vector<ServerThreadItem> threadPool;
    std::mutex tpLock; // threadPoolLock
    std::thread gameThread;
    GameServer game;
    addrinfo opts, *info;

    bool verbose;
};
