#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <iterator>
#include <functional>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <cstdint>
#include <memory>
#include <unistd.h>
#include <fcntl.h>

#include "socketwrapper.hpp"
#include "command_constants.hpp"
#include "chunkgroup.hpp"

struct ConnectionError: public std::exception
{
    ConnectionError(const char* error)
        : std::exception(), error{error} {};
    virtual const char* what() const throw() { return error; }
private:
    const char* error;
};

struct ClientSocket
{
    ClientSocket(const char* address, const uint16_t port);
    ~ClientSocket();

    void checkSocket(std::function<void(void)>& disconnectCallback, ChunkGroup& chunks);
    
    int fd;
private:
    void closeSocket();

    std::string address;
    const uint16_t port;
    
    addrinfo hints, *serverinfo;
};
