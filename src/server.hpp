#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cstdint>
#include <exception>
#include <SDL2/SDL_thread.h>

#include "socketwrapper.hpp"

struct Server;
struct ServerThreadItem;
struct ThreadData;
struct NetworkError;

int Server_thread(void* data);

// Struct for thread items
struct ServerThreadItem
{
    uint16_t id;
    SDL_Thread* thread;
    uint32_t count;
};

// Error handling
enum NERROR_TYPE
{
    NRESOLUTION_ERROR = 1,
    NSOCKOPT_ERROR,
    NSOCKBIND_ERROR
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
    bool warning(const std::string msg);
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
private:
    std::vector<ServerThreadItem> threadPool;
    int fd;
    addrinfo opts, *info;
    socklen_t sin_size;
    
    bool verbose;
};
