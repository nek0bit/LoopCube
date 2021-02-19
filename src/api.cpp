#include "api.hpp"

void Api::sendEcho(const int fd, const std::string data)
{
    std::stringstream toSend{COMMAND_ECHO};
    toSend << " " << data;
    const std::string& res = toSend.str();
    send(fd, res.c_str(), res.size(), 0);
    
}

void Api::sendPlayerPos(const int fd, const double x, const double y)
{
    std::stringstream toSend{COMMAND_PLAYER_POS};
    toSend << " " << x << " " << y;
    const std::string& res = toSend.str();
    send(fd, res.c_str(), res.size(), 0);
}

void Api::sendRecvChunk(const int fd, const long int x, const long int y)
{
    std::stringstream toSend{COMMAND_GET_CHUNK};
    toSend << " " << x << " " << y;
    const std::string& res = toSend.str();
    send(fd, res.c_str(), res.size(), 0);
}
