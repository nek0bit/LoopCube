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
      threadPool{},
      tpLock{},
      exit{false},
      gameThread{},
      game{},
      fd{},
      opts{},
      info{nullptr},
      verbose{verbose}
{
    constexpr int QUEUE = 10;
    int yes = 1, no = 0, err;
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
    addrinfo* cur = info;
    for (; cur != nullptr; cur = cur->ai_next)
    {
        if ((fd = socket(cur->ai_family, cur->ai_socktype, cur->ai_protocol)) == -1)
        {
            ServLog::warn(std::string{"Couldn't create socket: "} +
                             std::string{strerror(errno)} + tryingOther);
            continue;
        }


        // Allow reuse of this file descriptor
        if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
        {
            close(fd);
            throw NetworkError(NSOCKOPT_ERROR, strerror(errno));
        }
        
#ifndef __NONODELAY__
        // Disables the Nagle algorithm
        if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &no, sizeof(no)) == -1)
        {
            close(fd);
            throw NetworkError(NSOCKOPT_ERROR, strerror(errno));
        }
#endif
        
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
        int flags;
        if ((flags = fcntl(fd, F_GETFL)) == -1)
        {
            throw NetworkError(NSOCKBIND_ERROR, strerror(errno));
        }
        else
            flags |= O_NONBLOCK;
        
        
        if (fcntl(fd, F_SETFL, flags) == -1)
        {
            throw NetworkError(NSOCKBIND_ERROR, strerror(errno));
        }
        
        verbose && ServLog::log(std::string{"Successfully opened socket at file descriptor "} +
                                std::to_string(fd) + std::string{"."});
        
#ifndef __NOIPLOG__
        address = getAddress(cur->ai_addr);
#endif
    }

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

    FD_ZERO(&checkaccept);
    FD_SET(fd, &checkaccept);
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

    verbose && ServLog::log("Waiting for GameServer thread to finish...");
    if (gameThread.joinable()) gameThread.join();
    
    shutdown(fd, SHUT_RDWR); // Stop all accept calls and stuff
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

void Server::startGameThread() noexcept
{
    Timer TPS = 30;
    
    while (exit.load() == false)
    {
        TPS.setTime();

        game.update(TPS);

        int calc = TPS.calcSleep();
        if (calc > 0) std::this_thread::sleep_for(std::chrono::milliseconds(TPS.calcSleep()));
    }
}

void Server::startServer(const size_t threadCount)
{
    // Start the game server + it's thread
    gameThread = std::thread(&Server::startGameThread, this);
    

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

        fd_set check_copy = checkaccept;
        timeval time{0, 200000};

        if (select(fd+1, &check_copy, NULL, NULL, &time) != -1 &&
            FD_ISSET(fd, &check_copy)) // Only polling one item
        {
            if ((connection = accept(fd, (sockaddr*)&incoming, &sin_size)) == -1)
            {
                if (exit.load() == true)
                    break;
                continue;
            }
            else
            {
                if (verbose)
                {
                    std::string msg = "Incoming connection at file descriptor ";
                    ServLog::log(msg + std::to_string(connection));
                }

                int flags;
                if ((flags = fcntl(connection, F_GETFL)) == -1)
                {
                    if (exit.load() == true)
                        break;
                    continue;
                }
                else
                    flags |= O_NONBLOCK;
            
            
                if (fcntl(connection, F_SETFL, flags) == -1)
                {
                    if (exit.load() == true)
                        break;
                    continue;
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
    }
}

void Server::handleCommand(char* buffer, ServerThreadItem& item, const size_t index)
{
    const int& fd = item.connections[index].fd;
    ConnectionData& data = item.connectionData[index];
    
    std::string msg = buffer;
    std::vector<std::string> msgSplit;
    
    // KEEP ME COMMENTED OUT
    // Telnet debugger
    //if(*(msg.end()-1)=='\n'&&*(msg.end()-2)=='\r'){msg.erase(msg.end()-2, msg.end());}

    // Split a string
    std::istringstream sm(msg);
    std::string back;
    while (std::getline(sm, back, ' '))
    {
        msgSplit.push_back(back);
    }
    // ... man I love this language

    //******************************
    // COMMANDS
    //******************************
    try
    {
        // *** SINGLE COMMANDS ***
        if (msg == COMMAND_QUIT)
        {
            // If our client is a good boy!
            removeConnection(item, index);
        }
        // *** ARG COMMANDS ***
        else if (msgSplit.at(0) == COMMAND_ECHO)
        {
            std::string comb = std::string{ACTION_ECHO} +
                combineString(msgSplit.begin()+1, msgSplit.end(), " ");
            send(fd, comb.c_str(), comb.length(), 0);
        }
        else if (msgSplit.at(0) == COMMAND_PLAYER_POS)
        {
            try
            {
                // Game internally uses doubles, not floats
                data.playerX = std::stod(msgSplit.at(1));
                data.playerY = std::stod(msgSplit.at(2));
            }
            catch (const std::invalid_argument& err) {}
        }
        else if (msgSplit.at(0) == COMMAND_GET_CHUNK)
        {
            try
            {
                const long int x = std::stol(msgSplit.at(1));
                const long int y = std::stol(msgSplit.at(2));
                std::vector<unsigned char> data = {ACTION_GET_CHUNK};
                std::vector<unsigned char> getChunkAt = game.checkChunkAt(x, y);
                data.insert(data.end(), getChunkAt.begin(), getChunkAt.end());
                send(fd, &data[0], data.size(), 0);
            }
            catch (const std::invalid_argument& err) {}
        }
        else if (msgSplit.at(0) == COMMAND_PLACE_BLOCK_ABSOLUTE)
        {
            try
            {
                const long id = std::stoi(msgSplit.at(1));
                const long chunkX = std::stoi(msgSplit.at(2));
                const long chunkY = std::stoi(msgSplit.at(3));
                const long x = std::stoi(msgSplit.at(4));
                const long y = std::stoi(msgSplit.at(5));
                game.modifyBlock(id, chunkX, chunkY, x, y, false);
            }
            catch (const std::invalid_argument& err) {}
        }
        else if (msgSplit.at(0) == COMMAND_DESTROY_BLOCK_ABSOLUTE)
        {
            try
            {
                const long chunkX = std::stoi(msgSplit.at(1));
                const long chunkY = std::stoi(msgSplit.at(2));
                const long x = std::stoi(msgSplit.at(3));
                const long y = std::stoi(msgSplit.at(4));
                game.modifyBlock(-1, chunkX, chunkY, x, y, true);
            }
            catch (const std::invalid_argument& err) {}
        }
    }
    catch (const std::out_of_range& err) {} // Do nothing
}

void Server::removeConnection(ServerThreadItem& item, const size_t index)
{   
    close(item.connections[index].fd);

    // Erase from connections list
    item.connections.erase(item.connections.begin() + index);
    item.connectionData.erase(item.connectionData.begin() + index);

    const std::string name = item.connectionData[index].username;
    ServLog::log(std::string{"Player "} +
                 (name == "" ? std::string{"???"} : name) +
                 std::string{" disconnected."});
    verbose && ServLog::info(std::to_string(item.connections.size()) +
                 std::string{"=?="} +
                 std::to_string(item.connectionData.size()));
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

            int pl = poll(&item.connections[0], item.connections.size(), 100);
            
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

                            removeConnection(item, i);
                        }
                        else // Data is recieved
                        {
                            // Cap data
                            buffer[bytes] = '\0';

                            handleCommand(buffer, item, i);
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
