#include "clientsocket.hpp"

ClientSocket::ClientSocket(const std::string address, const uint16_t port)
    : address{address},
      port{port}
{
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(address.c_str(), std::to_string(port).c_str(), &hints, &serverinfo) != 0)
    {
        throw ConnectionError("getaddrinfo: Failed to connect to server.");
    }

    addrinfo* cur = serverinfo;
    for (; cur != NULL; cur = cur->ai_next)
    {
        if ((fd = socket(cur->ai_family, cur->ai_socktype, cur->ai_protocol)) == -1)
        {
            std::cout << "what" <<std::endl;
            continue;
        }

        if (connect(fd, cur->ai_addr, cur->ai_addrlen) == -1)
        {
            closeSocket();
        } else break;
    }

    if (cur == NULL)
    {
        throw ConnectionError("lookup: Failed to connect to server.");
    }

    // Now we start recieving!
}

ClientSocket::~ClientSocket()
{
    closeSocket();
}

void ClientSocket::startListening()
{

}

void ClientSocket::closeSocket()
{
    close(fd);
}
