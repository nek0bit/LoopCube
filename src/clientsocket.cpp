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

    // Make socket non-blocking
    int flags;
    if ((flags = fcntl(fd, F_GETFL)) == -1)
        throw ConnectionError(strerror(errno));
    
    flags |= O_NONBLOCK;

    if (fcntl(fd, F_SETFL, flags) == -1)
        throw ConnectionError(strerror(errno));
    
#ifndef __NONODELAY__
    // Disables the Nagle algorithm, disables verification
    if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &no, sizeof(no)) == -1)
    {
        close(fd);
        throw ConnectionError(strerror(errno));
    }
#endif
}

ClientSocket::~ClientSocket()
{
    closeSocket();
}

void ClientSocket::checkSocket(ChunkGroup& chunks)
{
    constexpr size_t BUF_SIZE = 4096;
    int bc;
    unsigned char buf[BUF_SIZE];
    
    if ((bc = recv(fd, buf, BUF_SIZE-1, 0)) != -1)
    {
        buf[bc] = '\0';
        unsigned char* noByte = buf;
        noByte++; // Skips the action
        switch (buf[0])
        {
        case ACTION_ECHO:
            break;
        case ACTION_GET_CHUNK:
        {
            std::vector<unsigned char> deserializeMe{std::begin(buf), std::end(buf)};
            chunks.loadFromDeserialize(deserializeMe, true);
            chunks.chunkReady = true;
        }
        break;
        case ACTION_PLACE_BLOCK:
            std::cout << "Block placed" << std::endl;
            break;
        case ACTION_DESTROY_BLOCK:
            std::cout << "Block removed" << std::endl;
            break;
        default:
            break;
        }
    }
}

void ClientSocket::closeSocket()
{
    close(fd);
}
