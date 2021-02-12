#pragma once
#include <iostream>
#include <string>
#include <algorithm>
#include <cstring>
#include <cstdint>
#include <exception>

#include "socketwrapper.hpp"

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

    void startServer(const uint16_t threadCount = 1); // Note: Blocking
private:
    int fd;
    addrinfo opts, *info;
    socklen_t sin_size;
    
    bool verbose;
};
