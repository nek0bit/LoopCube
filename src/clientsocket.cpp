#include "clientsocket.hpp"

ClientSocket::ClientSocket(const char* address, const uint16_t port)
    : address{},
      port{port}
{
    int no = 0;
    std::string strPort = std::to_string(port);

    if (address != NULL) this->address = std::string(address);
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(address, strPort.c_str(), &hints, &serverinfo) != 0)
    {
        throw ConnectionError("getaddrinfo: Failed to connect to server.");
    }

    addrinfo* cur = serverinfo;
    for (; cur != nullptr; cur = cur->ai_next)
    {
        if ((fd = socket(cur->ai_family, cur->ai_socktype, cur->ai_protocol)) == -1)
        {
            continue;
        }

        if (connect(fd, cur->ai_addr, cur->ai_addrlen) != -1) break;
    }

    if (cur == nullptr)
        throw ConnectionError("lookup: Failed to connect to server.");

    
#ifndef __NONODELAY__
    // Disables the Nagle algorithm, disables verification
    if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &no, sizeof(no)) == -1)
    {
        close(fd);
        throw ConnectionError(strerror(errno));
    }
#endif

    FD_ZERO(&checksock);

    FD_SET(fd, &checksock);
}

ClientSocket::~ClientSocket()
{
    closeSocket();
}

void ClientSocket::checkSocket(ChunkGroup& chunks)
{
    
    // Copy checksock, since select corrupts the value passed into it
    fd_set check_copy = checksock;
    
    timeval time{0, 0};
    
    if (select(fd+1, &check_copy, NULL, NULL, &time) != -1)
    {
        if (FD_ISSET(fd, &check_copy))
        {
            // *shrug*
            recv(fd, NULL, 0, 0);
        }
    }
}

void ClientSocket::closeSocket()
{
    close(fd);
}
