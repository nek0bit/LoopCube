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

#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

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
    ClientSocket(const std::string address, const uint16_t port);
    ~ClientSocket();

private:
    void closeSocket();

    const std::string address;
    const uint16_t port;
    
    int fd;
    addrinfo hints, *serverinfo;
    
};
