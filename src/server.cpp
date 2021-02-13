#include "server.hpp"

bool ServLog::warn(const std::string msg)
    { std::cout << "! [Warn] " << msg << std::endl; return true; }
bool ServLog::error(const std::string msg)
    { std::cout << "!!! [Error] " << msg << std::endl; return true; }
bool ServLog::info(const std::string msg)
    { std::cout << "? [Info] " << msg << std::endl; return true; }
bool ServLog::log(const std::string msg)
    { std::cout << "[Log] " << msg << std::endl; return true; }

Server::Server(const uint32_t port, bool verbose)
    : port{port},
      address{},
      fd{},
      opts{},
      info{nullptr},
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
            ServLog::warn(std::string{"Couldn't create socket: "} +
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
            ServLog::warn(std::string{"Failed to bind: "} +
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
        
#ifndef __NOIPLOG__
        address = getAddress(cur->ai_addr);
#endif
    }    
}

Server::~Server()
{
    ServLog::log("Shutting down server...");

    exit = true;

    // Close all current connections
    tpLock.lock();
    for (ServerThreadItem& item: threadPool)
    {
        for (pollfd& con: item.connections)
        {
            verbose && ServLog::log(std::string{"Closing connection "} + std::to_string(con.fd));
            close(con.fd);
        }
    }
    tpLock.unlock();

    // Join threadpool threads
    !verbose && ServLog::log("Waiting for threads to finish...");
    for (ServerThreadItem& item: threadPool)
    {
        verbose && ServLog::log(std::string{"Waiting for thread #"} +
                                std::to_string(item.id) + std::string{" to finish..."});

        
        if (item.thread.joinable()) item.thread.join();
    }
    
    close(fd);
}

// sockaddr_in6 doesn't exist on all systems... so just for now
// there is an option to disable this function
#ifndef __NOIPLOG__
std::string Server::getAddress(sockaddr* info)
{
    
    char addr[INET6_ADDRSTRLEN];
    switch(info->sa_family)
    {
    case AF_INET:
        inet_ntop(AF_INET,
                  &((sockaddr_in*)info)->sin_addr,
                  addr, sizeof(addr));
        break;
    case AF_INET6:
        inet_ntop(AF_INET6,
                  &((sockaddr_in6*)info)->sin6_addr,
                  addr, sizeof(addr));
        break;
    default:
        break;
    }
    return addr;
}
#endif

void Server::startServer(const size_t threadCount)
{
    
    constexpr int QUEUE = 10;

    // Start listening for connections
    if (listen(fd, QUEUE) == -1)
    {
        std::string msg = "Failed to listen: ";
        msg += strerror(errno);
        throw NetworkError(NLISTEN_ERROR, msg.c_str());
    }
    else
    {
        std::string msg = "Server listening at ";
        msg += address + std::string(":") + std::to_string(port);
        ServLog::info(msg);
    }

    // Create threadpool
    for (size_t i = 0; i < threadCount; ++i)
    {
        tpLock.lock();
        uint16_t id = i + 1;
        threadPool.emplace_back(ServerThreadItem{id,
                std::thread(&Server::serverThread, this, i),
                0, {}, {}});
        tpLock.unlock();
    }

    while (!exit.load())
    {
        sockaddr_storage incoming;
        socklen_t sin_size = sizeof(incoming);
        int connection;

        if ((connection = accept(fd, (sockaddr*)&incoming, &sin_size)) == -1)
        {
            std::string msg = "Failed to accept connection: ";
            msg += strerror(errno);
            ServLog::warn(msg);
            continue;
        }
        else
        {
            if (verbose)
            {
                std::string msg = "Incoming connection at file descriptor ";
                ServLog::log(msg + std::to_string(connection));
            }
        }
        
#ifndef __NOIPLOG__
        // Print information regarding new connection
        std::string ipMsg = "Server: Got connection from ";
        ipMsg += getAddress((sockaddr*)&incoming);
        ServLog::log(ipMsg);
#endif
        
        // Add to smallest thread
        ServerThreadItem& low = minThreadCount();

        tpLock.lock();
        // Add poll to pollfd vector
        pollfd newPoll;
        newPoll.fd = connection;
        newPoll.events = POLLIN;
        
        low.connections.push_back(newPoll);
        low.connectionData.push_back({"", 0.0, 0.0});
        tpLock.unlock();
    }
}

void Server::handleCommand(char* buffer, ConnectionData& data)
{
    std::string msg = buffer;
}

ServerThreadItem& Server::minThreadCount()
{
    uint32_t count = -1;
    ServerThreadItem* ret = nullptr;
    for (ServerThreadItem& item: threadPool)
    {
        if (count > item.count || count == static_cast<uint32_t>(-1))
        {
            count = item.count;
            ret = &item;
        }
    }
    ret->count++;
    
    return *ret;
}

void Server::serverThread(const size_t index) noexcept
{
    char buffer[256];
    while (!exit.load())
    {
        tpLock.lock(); // Ensure threadPool has been created
                       // also ensures threadPool cannot be modified
        try
        {
            ServerThreadItem& item = threadPool.at(index);
            tpLock.unlock();
            
            int pl = poll(&item.connections[0], item.connections.size(), 5000);
            
            if (pl == -1)
            {
                ServLog::error(std::string{"Poll: "} + std::string{strerror(errno)});
            }
            else if (pl != 0) // Not a timeout
            {
                // Go through each connection
                for (size_t i = 0; i < item.connections.size(); ++i)
                {
                    const pollfd& con = item.connections[i];
                    // Recieved data
                    if (con.revents & POLLIN)
                    {
                        int bytes = recv(con.fd, buffer, sizeof(buffer), 0);

                        // Connection closed... or error occured
                        if (bytes <= 0)
                        {
                            if (bytes == 0)
                            {
                                ServLog::info(std::string{"Disconnect from fd "} +
                                              std::to_string(con.fd));
                            }
                            else
                            {
                                ServLog::warn(std::string{"Recv: "} + std::string{strerror(errno)});
                            }

                            close(con.fd);
                            
                            item.connections.erase(item.connections.begin() + i);
                        }
                        else // Data is recieved
                        {
                            // Cap data
                            buffer[bytes] = '\0';

                            handleCommand(buffer, item.connectionData[i]);
                        }
                    }
                }
            }
        }
        catch (const std::out_of_range& err)
        {
            tpLock.unlock();
            continue;
        }
    }
}
