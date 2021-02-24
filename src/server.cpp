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
    : exit{false},
      fd{},
      port{port},
      address{},
      threadPool{},
      tpLock{},
      gameThread{},
      checkaccept{},
      game{},
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
                low.connectionData.push_back({"Unnamed", 0, 0});
                tpLock.unlock();
            }
        }        
    }
}

void Server::commandGetChunk(const int fd, const std::vector<unsigned char>& value)
{
    // Extract the size from value
    const uint8_t chunkXSize = value.at(2);
    const uint8_t chunkYSize = value.at(2+chunkXSize+1);

    // Deserialize the data
    long chunkX = Generic::deserializeSigned<std::vector<unsigned char>, long>
        (value, 3, chunkXSize);
    long chunkY = Generic::deserializeSigned<std::vector<unsigned char>, long>
        (value, 2+chunkXSize+2, chunkYSize);

    // Encode full size
    // push back ACTION_GET_CHUNK
    std::vector<unsigned char> getChunkAt = game.checkChunkAt(chunkX, chunkY);
                    
    // add 4 to account for 2 bytes of data size, 1 byte of 0xff at end, and 1 byte action
    const uint16_t dataSize = getChunkAt.size()+4;

    std::vector<unsigned char> data{};

    // I don't expect data to be larger then 65535, so 2 bytes should be enough...
    Generic::serializeUnsigned(dataSize, 2, [&data](uint8_t back)
        { data.push_back(back); });

    // Push back action name for client to use
    data.push_back(ACTION_GET_CHUNK);

    // Insert chunk data
    data.insert(data.end(), getChunkAt.begin(), getChunkAt.end());

    data.push_back(0xff); // End
    
    send(fd, &data[0], dataSize, 0);
}

void Server::commandPlaceBlockAbsolute(const int fd, const std::vector<ServerThreadItem>& threadPool,
                                       const std::vector<unsigned char>& value)
{
    // Notice a pattern here...
    const uint8_t idSize = value.at(2);
    const uint8_t chunkXSize = value.at(2+idSize+1);
    const uint8_t chunkYSize = value.at(2+idSize+chunkXSize+2);
    const uint8_t blockXSize = value.at(2+idSize+chunkXSize+chunkYSize+3);
    const uint8_t blockYSize = value.at(2+idSize+chunkXSize+chunkYSize+blockXSize+4);

    uint32_t id = Generic::deserializeUnsigned<std::vector<unsigned char>, uint32_t>
        (value, 3, idSize);
    int64_t chunkX = Generic::deserializeSigned<std::vector<unsigned char>, int64_t>
        (value, 2+idSize+2, chunkXSize);
    int64_t chunkY = Generic::deserializeSigned<std::vector<unsigned char>, int64_t>
        (value, 2+idSize+chunkXSize+3, chunkYSize);
    uint8_t blockX = Generic::deserializeUnsigned<std::vector<unsigned char>, uint8_t>
        (value, 2+idSize+chunkXSize+chunkYSize+4, blockXSize);
    uint8_t blockY = Generic::deserializeUnsigned<std::vector<unsigned char>, uint8_t>
        (value, 2+idSize+chunkYSize+chunkYSize+blockXSize+5, blockYSize);
                        
    game.modifyBlock(fd, threadPool, id, chunkX, chunkY, blockX, blockY, false);
}

void Server::commandDestroyBlockAbsolute(const int fd, const std::vector<ServerThreadItem>& threadPool,
                                         const std::vector<unsigned char>& value)
{
    // Notice a pattern here...
    const uint8_t chunkXSize = value.at(2);
    const uint8_t chunkYSize = value.at(2+chunkXSize+1);
    const uint8_t blockXSize = value.at(2+chunkXSize+chunkYSize+2);
    const uint8_t blockYSize = value.at(2+chunkXSize+chunkYSize+blockXSize+3);

    // Deserialize the chunk and block data in order
    int64_t chunkX = Generic::deserializeSigned<std::vector<unsigned char>, int64_t>
        (value, 3, chunkXSize);
    
    int64_t chunkY = Generic::deserializeSigned<std::vector<unsigned char>, int64_t>
        (value, 2+chunkXSize+2, chunkYSize);
    
    uint8_t blockX = Generic::deserializeUnsigned<std::vector<unsigned char>, uint8_t>
        (value, 2+chunkXSize+chunkYSize+3, blockXSize);
    
    uint8_t blockY = Generic::deserializeUnsigned<std::vector<unsigned char>, uint8_t>
        (value, 2+chunkYSize+chunkYSize+blockXSize+4, blockYSize);
                                                            
    game.modifyBlock(fd, threadPool, -1, chunkX, chunkY, blockX, blockY, true);
}

void Server::commandPlayerPosition(const int fd, const std::vector<unsigned char>& value,
                                   const std::vector<ServerThreadItem>& threadPool,
                                   ConnectionData& connectionData)
{
    // Data to send back to client(s)
    std::vector<unsigned char> sendBack{};

    // Get index of size values
    const uint8_t playerXSize = value.at(2);
    const uint8_t playerYSize = value.at(2+playerXSize+1);

    // Deserialize player
    int playerX = Generic::deserializeSigned<std::vector<unsigned char>, long>
        (value, 3, playerXSize);
    int playerY = Generic::deserializeSigned<std::vector<unsigned char>, long>
        (value, 2+playerXSize+2, playerYSize);

    // Store data into connectionData
    connectionData.roundPlayerX = playerX;
    connectionData.roundPlayerY = playerY;

    const uint16_t dataSize = sizeof(fd) +
        sizeof(playerX) + sizeof(playerY) + 7;
                        
    Generic::serializeUnsigned(dataSize, 2, [&sendBack](uint8_t back)
        { sendBack.push_back(back); });

    sendBack.push_back(static_cast<unsigned char>(ACTION_PLAYER_UPDATE) & 0xff);

    sendBack.push_back(sizeof(fd));
    Generic::serializeSigned(fd, sizeof(fd), [&sendBack](uint8_t back) {
        sendBack.push_back(back);
    });

    sendBack.push_back(sizeof(playerX));
    Generic::serializeSigned(playerX, sizeof(playerX), [&sendBack](uint8_t back) {
        sendBack.push_back(back);
    });

    sendBack.push_back(sizeof(playerY));
    Generic::serializeSigned(playerY, sizeof(playerY), [&sendBack](uint8_t back) {
        sendBack.push_back(back);
    });

    sendBack.push_back(0xff);

    for (const ServerThreadItem& item: threadPool)
    {
        for (const pollfd& pfd: item.connections)
        {
            if (pfd.fd != fd) send(pfd.fd, &sendBack[0], dataSize, 0);
        }
    }
}

void Server::handleCommand(ServerThreadItem& item, const size_t index)
{
    constexpr size_t MAX_BUFFER = 2048;
    const int& fd = item.connections[index].fd;
    ConnectionData& connectionData = item.connectionData[index];
    int res;

    // Fetch first byte, see if we even can
    do
    {
        unsigned char buffer[MAX_BUFFER];
        int sizeByte = recv(fd, buffer, 1, 0);
        int dataLength = buffer[0];

        // Connection closed... or error occured
        if (sizeByte <= 0)
        {
            if (sizeByte == 0)
            {
                ServLog::info(std::string{"Disconnect from fd "} +
                              std::to_string(fd));
                removeConnection(item, index);
            }
            return;
        }

        // Read offset first
        res = recv(fd, &buffer[1],
                   static_cast<size_t>(dataLength) >= MAX_BUFFER ?
                   MAX_BUFFER-1 : dataLength, 0);
        
        // Convert it into a vector
        std::vector<unsigned char> value(std::begin(buffer), std::begin(buffer)+dataLength);        
        

        // Read values until we run out of data to read!
        //******************************
        // COMMANDS
        //******************************
        try
        {
            // This should be 0xff!
            if (value.at(dataLength-1) != 0xff)
            {
                // Perhaps a mistake was made?
                return;
            }
            
            // Read the first byte
            switch(value.at(1))
            {
            case COMMAND_QUIT:
                removeConnection(item, index);
                break;
            case COMMAND_GET_CHUNK:
                commandGetChunk(fd, value);
                break;
            case COMMAND_PLACE_BLOCK_ABSOLUTE:
                commandPlaceBlockAbsolute(fd, threadPool, value);
                break;                    
            case COMMAND_DESTROY_BLOCK_ABSOLUTE:
                commandDestroyBlockAbsolute(fd, threadPool, value);
                break;
            case COMMAND_PLAYER_POS:
                commandPlayerPosition(fd, value, threadPool, connectionData);
                break;
            default:
                break;
            }
        }
        catch (const std::out_of_range& err) {
            break;
        }
    } while (!(res == -1 || res == 0)); // Loop until there is no data to read
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
    while (!exit.load())
    {
        std::lock_guard<std::mutex> lg(tpLock);
        
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
                        handleCommand(item, i);
                    }
                }
            }
        }
        catch (const std::out_of_range& err) { continue; }
    }
}
