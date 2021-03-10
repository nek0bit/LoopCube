#include "clientsocket.hpp"

ClientSocket::ClientSocket(const char* address, const uint16_t port)
    : address{},
      port{port}
{
    int no = 0;
    std::string strPort = std::to_string(this->port);

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

void ClientSocket::checkSocket(std::function<void(void)>& disconnectCallback,
                               PlayerGroup& players, ChunkGroup& chunks)
{
    constexpr size_t BUF_SIZE = 2048;
    int res;

    do
    {
        unsigned char buffer[BUF_SIZE];
        int sizeByte = recv(fd, buffer, 2, 0);

        if (sizeByte == 0)
        {
            disconnectCallback();
            return;
        }

        if (sizeByte <= 0) return;

        std::vector<unsigned char> readInto(std::begin(buffer), std::begin(buffer)+2);

        uint16_t dataLength = Generic::deserializeUnsigned<std::vector<unsigned char>, uint16_t>
            (readInto, 0, 2);

        // Receive rest of data
        res = recv(fd, &buffer[2], dataLength-2, 0);

        if (res <= 0) return;
            
        std::vector<unsigned char> value(std::begin(buffer)+2, std::begin(buffer)+res+2);

        if (value.at(dataLength-3) != 0xff) return;
        
        try
        {   
            switch(value.at(0))
            {
            case ACTION_PLAYER_UPDATE:
                actionPlayerUpdate(value, players);
                break;
            case ACTION_GET_CHUNK:
            {
                chunks.loadFromDeserialize(value);
                chunks.chunkReady = true;
            }
            break;
            case ACTION_PLACE_BLOCK:
            case ACTION_DESTROY_BLOCK:
                modifyBlock(value, chunks);
                break;
            default:
                break;
            }
        }
        catch (const std::out_of_range& err) {}
    } while (res > 0);
}

void ClientSocket::actionPlayerUpdate(const std::vector<unsigned char>& value, PlayerGroup& players)
{
    const uint8_t idSize = value.at(1);
    const uint8_t playerXSize = value.at(idSize+2);
    const uint8_t playerYSize = value.at(idSize+playerXSize+3);

    int id = Generic::deserializeSigned<std::vector<unsigned char>, long>
        (value, 2, idSize);
    int playerX = Generic::deserializeSigned<std::vector<unsigned char>, long>
        (value, idSize+3, playerXSize);
    int playerY = Generic::deserializeSigned<std::vector<unsigned char>, long>
        (value, idSize+playerXSize+4, playerYSize);

    players.updateSpecificPlayer(id, playerX, playerY);
}

void ClientSocket::modifyBlock(const std::vector<unsigned char>& value, ChunkGroup& chunks)
{
    // Extract info
    try
    {
        const int chunkXSize = value.at(1);
        const int chunkYSize = value.at(chunkXSize+2);
        const uint8_t blockX = value.at(chunkXSize + chunkYSize + 3);
        const uint8_t blockY = value.at(chunkXSize + chunkYSize + 4);

        // Extract chunkX and chunkY
        long chunkX = Generic::deserializeSigned<std::vector<unsigned char>,
                                                 long>(value, 2, chunkXSize);
        long chunkY = Generic::deserializeSigned<std::vector<unsigned char>,
                                                 long>(value,
                                                       chunkXSize + 3,
                                                       chunkYSize);

        Chunk* chk = chunks.getChunkAt(chunkX, chunkY);
        if (chk != nullptr)
        {
            if (value.at(0) == ACTION_PLACE_BLOCK)
            {
                chk->placeBlock(0, blockX, blockY);
            }
            else // Destroy Block
            {
                chk->destroyBlock(blockX, blockY);
            }

            chk->generateChunkMesh();
        }
    }
    catch (const std::out_of_range& err)
    {
        std::cerr << "[Warning] Recieved incorrect block" << std::endl;
    }
}

void ClientSocket::closeSocket()
{
    close(fd);
}
