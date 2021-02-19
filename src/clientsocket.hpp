#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <cstdint>
#include <memory>
#include <unistd.h>

#include "socketwrapper.hpp"

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

    int fd;
private:
    void closeSocket();

    std::string address;
    const uint16_t port;
    
    addrinfo hints, *serverinfo;
    
};
