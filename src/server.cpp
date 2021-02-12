#include "server.hpp"

bool ServLog::warning(const std::string msg)
    { std::cout << "! [Warning] " << msg << std::endl; return true; }
bool ServLog::error(const std::string msg)
    { std::cout << "!!! [Error] " << msg << std::endl; return true; }
bool ServLog::info(const std::string msg)
    { std::cout << "? [Info] " << msg << std::endl; return true; }
bool ServLog::log(const std::string msg)
    { std::cout << "[Log] " << msg << std::endl; return true; }

Server::Server(const uint32_t port, bool verbose)
    : fd{},
      opts{},
      info{nullptr},
      sin_size{},
      verbose{verbose}
{
    int one = 1, err;
    // Clear hints struct
    memset(&opts, 0, sizeof opts);
    opts.ai_family = AF_UNSPEC;
    opts.ai_socktype = SOCK_STREAM;
    opts.ai_flags = AI_PASSIVE;

    // get opt and info
    if ((err = getaddrinfo(NULL, std::to_string(port).c_str(), &opts, &info)) != 0)
    {
        throw NetworkError(NRESOLUTION_ERROR, gai_strerror(err));
    }

    // Loop through resolution entries
    std::string tryingOther = (verbose ? "\nTrying another one if available..." : "");
    addrinfo* cur = nullptr;
    for (cur = info; cur != nullptr; cur = cur->ai_next)
    {
        if ((fd = socket(cur->ai_family, cur->ai_socktype, cur->ai_protocol)) == -1)
        {
            ServLog::warning(std::string{"Couldn't create socket: "} +
                             std::string{strerror(errno)} + tryingOther);
            continue;
        }

        // Allow reuse of this file descriptor
        if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one)) == -1)
        {
            close(fd);
            throw NetworkError(NSOCKOPT_ERROR, strerror(errno));
        }

        if (bind(fd, cur->ai_addr, cur->ai_addrlen) == -1)
        {
            close(fd);
            ServLog::warning(std::string{"Failed to bind: "} +
                             std::string{strerror(errno)} + tryingOther);
        }
        else
        {
            break;
        }
    }

    freeaddrinfo(info);

    if (!cur)
    {
        throw NetworkError(NSOCKBIND_ERROR, "Failed to open socket or bind all available addresses!");
    }
    else
    {
        verbose && ServLog::log(std::string{"Successfully opened socket at file descriptor "} +
                                std::to_string(fd) + std::string{"."});
    }

    // TODO listening (probably done in other thread)
}

Server::~Server()
{
}

void Server::startServer(const uint16_t threadCount)
{
    
}
