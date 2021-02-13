#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
#include <algorithm>
#include <cstring>
#include <cstdint>
#include <exception>
#include <poll.h>

#include "socketwrapper.hpp"

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
private:
    void handleCommand(char* buffer, ConnectionData& data);
    ServerThreadItem& minThreadCount();
#ifndef __NOIPLOG__
    std::string getAddress(sockaddr* info);
#endif
    const uint32_t port;
    std::string address;
    std::vector<ServerThreadItem> threadPool;
    std::mutex tpLock; // threadPoolLock
    std::atomic<bool> exit;
    int fd;
    addrinfo opts, *info;

    bool verbose;
};
