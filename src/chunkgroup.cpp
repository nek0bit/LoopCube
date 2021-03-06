#include "chunkgroup.hpp"

//***************************************
// _ChunkDataSplit: For vertical chunks
//***************************************
_ChunkDataSplit::_ChunkDataSplit(long int x, LoadPtr& loadPtr, LoadDistance& loadDistance,
                                 int& fd, bool& isFdSet, bool& chunkReady, GLuint shader)
    : loadPtr{loadPtr},
      loadDistance{loadDistance},
      loadedChunks{},
      left{nullptr},
      right{nullptr},
      x{x},
      chunkReady{chunkReady},
      shader{shader},
      isFdSet{isFdSet},
      fd{fd},
      isClient{true},
      chunkGen{nullptr}
{
    prepareLoaded();
}

_ChunkDataSplit::_ChunkDataSplit(long int x, LoadPtr& loadPtr, LoadDistance& loadDistance,
                                 std::shared_ptr<ChunkGen> chunkGen, int& fd,
                                 bool& isFdSet, bool& chunkReady, GLuint shader)
    : loadPtr{loadPtr},
      loadDistance{loadDistance},
      loadedChunks{},
      left{nullptr},
      right{nullptr},
      x{x},
      chunkReady{chunkReady},
      shader{shader},
      isFdSet{isFdSet},
      fd{fd},
      isClient{false},
      chunkGen{chunkGen}
{}

_ChunkDataSplit::~_ChunkDataSplit()
{}

// Generates a chunk if it isn't loaded
std::unordered_map<long int, ChunkData>::iterator
_ChunkDataSplit::checkGenerate(long int y,
                               std::shared_ptr<Chunk> toInsert,
                               bool isClient)
{
    auto ind = data.find(y);
    // Check if element doesn't exists
    if (ind == data.end())
    {
        // If we're a client, send a request to fetch the chunk from the server
        // We'll recieve it later and handle it
        if (isClient)
        {
            if (isFdSet && chunkReady)
            {
                Api::sendRecvChunk(fd, x, y);
                chunkReady = false;
                return data.end();
            }
            else
            {
                return data.end();
            }
        }
        else // Server side, generate the chunk
        {
            // Generate the chunk
            data.insert({y, ChunkData{true, toInsert ? toInsert :
                        std::make_shared<Chunk>(shader, chunkGen, x, y)}});
            auto current = data.find(y);

            updateBorderedChunks(current, y);
        
            return current;
        }
    }
    return ind;
}

// Clears and reupdates the loadedChunks vector
void _ChunkDataSplit::updateLoaded()
{
    for (size_t i = 0; i < loadedChunks.size(); ++i)
    {
        const int chunkPos = -loadPtr.y + i;
        Chunk* dataReceived = getData(chunkPos);
        
        // Generate if nullptr (might want to remove this later)
        if (dataReceived == nullptr)
        {
            auto get = checkGenerate(chunkPos, nullptr, isClient);
            
            if (get != data.end()) dataReceived = get->second.data.get();
        }

        if (dataReceived != nullptr) loadedChunks[i] = dataReceived;
    }
}

// Updates the left, right, top, and bottom chunks pointers
void _ChunkDataSplit::updateBorderedChunks(std::unordered_map<long int,
                                           ChunkData>::iterator current, const long y)
{
    if (current == data.end()) return;
    Chunk* newChunk = current->second.data.get();

    auto chunkAbove = data.find(y - 1),
        chunkBelow = data.find(y + 1);

    if (chunkAbove != data.end())
    {
        newChunk->borders.top = chunkAbove->second.data.get();
        newChunk->regenBlockBorders();
        chunkAbove->second.data->borders.bottom = newChunk;
        chunkAbove->second.data->regenBlockBorders();
    }
        
    if (chunkBelow != data.end())
    {
        newChunk->borders.bottom = chunkBelow->second.data.get();
        newChunk->regenBlockBorders();
        chunkBelow->second.data->borders.top = newChunk;
        chunkBelow->second.data->regenBlockBorders();
    }

    if (left != nullptr)
    {
        Chunk* chunkLeft = left->getData(y);
        if (chunkLeft != nullptr)
        {
            chunkLeft->borders.right = newChunk;
            chunkLeft->regenBlockBorders();
            newChunk->borders.left = chunkLeft;
            newChunk->regenBlockBorders();
        }
    }

    if (right != nullptr)
    {
        Chunk* chunkRight = right->getData(y);
        if (chunkRight != nullptr)
        {
            chunkRight->borders.left = newChunk;
            chunkRight->regenBlockBorders();
            newChunk->borders.right = chunkRight;
            newChunk->regenBlockBorders();
        }
    }
}

// Update all loaded chunks in the split
#ifndef __HEADLESS
void _ChunkDataSplit::updateSplit(const Camera& camera)
{
    // TODO Remove me, just for debugging
    updateLoaded();
    
    for (auto& chunk: loadedChunks)
    {
        if (chunk != nullptr) chunk->updateAll(camera);
    }
}

// Renders all loaded chunks in the split if visible
void _ChunkDataSplit::renderSplit(const Graphics& graphics, const Camera& camera) const
{
    for (auto& chunk: loadedChunks)
    {
        if (chunk != nullptr) chunk->renderChunk(graphics, camera);
    }
}
#endif

Chunk* _ChunkDataSplit::getData(long int y)
{
    try { return data.at(y).data.get(); }
    catch (std::out_of_range& err) { return nullptr; }
}

// Clears and resizes loadedChunks
void _ChunkDataSplit::prepareLoaded()
{
    loadedChunks.clear();
    loadedChunks.resize(loadDistance.y);
}

//***************************************************
// ChunkGroup: For horizontal vertical chunks,
// Handles most abstractions
//***************************************************

ChunkGroup::ChunkGroup(GLuint shader)
    : loadPtr{0, 0},
      loadDistance{constants::loadDistance},
      chunkReady{true},
      shader{shader},
      isFdSet{false},
      fd{0},
      isClient{true},
      chunkGen{nullptr},
      loadedSplits{},
      data{}
{
    prepareLoaded();
    updateLoaded();
}

ChunkGroup::ChunkGroup(std::shared_ptr<ChunkGen> chunkGen, GLuint shader)
    :  loadPtr{0, 0},
       loadDistance{constants::loadDistance},
       chunkReady{false},
       shader{shader},
       isFdSet{false},
       fd{0},
       isClient{false},
       chunkGen{chunkGen},
       loadedSplits{},
       data{}
{}

ChunkGroup::~ChunkGroup()
{}
      
void ChunkGroup::updateLoaded()
{
    for (size_t i = 0; i < loadedSplits.size(); ++i)
    {
        const int chunkPos = loadPtr.x + i;
        _ChunkDataSplit* dataReceived = getData(chunkPos);
        
        // Generate if nullptr (might want to remove this later)
        if (dataReceived == nullptr)
        {
            auto get = checkSplitGenerate(chunkPos);
            if (get != data.end()) dataReceived = get->second.get();
        }
        
        if (dataReceived != nullptr) loadedSplits[i] = dataReceived;
    }    
}

void ChunkGroup::prepareLoaded()
{
    loadedSplits.clear();
    loadedSplits.resize(loadDistance.x);
}

// Cleaner way to get at data
_ChunkDataSplit* ChunkGroup::getData(long int x) const
{
    try { return data.at(x).get(); }
    catch (const std::out_of_range& err) { return nullptr; }
}

#ifndef __HEADLESS
void ChunkGroup::update(const Camera& camera)
{
    // TODO Remove me, just for testing things out
    updateLoaded();

    for (auto& split: loadedSplits)
    {
        if (split != nullptr) split->updateSplit(camera);
    }
}

void ChunkGroup::render(const Graphics& graphics, const Camera& camera) const
{
    for (auto& split: loadedSplits)
    {
        if (split != nullptr) split->renderSplit(graphics, camera);
    }
}
#endif

// Wrapper for multiple getData calls
Chunk* ChunkGroup::getChunkAt(const long int x, const long int y) const
{
    _ChunkDataSplit* call = getData(x);
    
    return call == nullptr ? nullptr : getData(x)->getData(y);
}

std::vector<Chunk*> ChunkGroup::isWithinChunks(const glm::vec2& vec, const glm::vec2& size) const
{
    ChunkPos inChunk = posToChunkPos(vec.x, vec.y);
    GridCollision_t col = Generic::gridCollision(constants::chunkWidth * constants::blockW,
                                                 constants::chunkHeight * constants::blockH,
                                                 vec, size);

    std::vector<Chunk*> ch{};

    for (size_t i = 0; i < col.width + 1; ++i)
    {
        for (size_t j = 0; j < col.height + 1; ++j)
        {
            ch.emplace_back(getChunkAt(inChunk.x + i, inChunk.y + j));
        }
    }

    return ch;
}

// BUG freezes on invalid data
void ChunkGroup::loadFromDeserialize(std::vector<unsigned char>& value, int start)
{
    int at = start;
    
    const uint8_t chunkXSize = value.at(at);
    const uint8_t chunkYSize = value.at(at+chunkXSize+1);

    // Deserialize chunk positions
    int resX = Generic::deserializeSigned<std::vector<unsigned char>, long>(value, at+1, chunkXSize);
    int resY = Generic::deserializeSigned<std::vector<unsigned char>, long>(value, at+chunkXSize+2, chunkYSize);
    
    auto splitX = checkSplitGenerate(resX);
    
    if (splitX != data.end())
    {
        auto splitY = splitX->second;

        splitY->checkGenerate(resY,
                              std::make_shared<Chunk>(shader, nullptr, resX, resY),
                              false);
        
        auto chunkAt = splitY->getData(resY);
        chunkAt->deserialize(value, true);
    }
}

void ChunkGroup::generateChunkAt(const long x, const long y)
{
    auto splitX = checkSplitGenerate(x);
    splitX->second->checkGenerate(y, nullptr, isClient);
}

void ChunkGroup::setFd(const int fd)
{
    this->fd = fd;
    isFdSet = true;
}

ChunkPos ChunkGroup::posToChunkPos(double x, double y) const
{
    const int absSizeX = constants::chunkWidth * constants::blockW;
    const int absSizeY = constants::chunkHeight * constants::blockH;
    
    // Prevent -0 from being a value (which is really still 0)
    if (x < 0) x -= constants::chunkWidth * constants::blockW + 1;
    if (y < 0) y -= constants::chunkHeight * constants::blockH + 1;
    
    return {static_cast<long>(x / absSizeX),
        static_cast<long>(y / absSizeY)};
}

std::unordered_map<long int, std::shared_ptr<_ChunkDataSplit>>::iterator
ChunkGroup::checkSplitGenerate(long int x)
{
    auto ind = data.find(x);
    // Check if element doesn't exists
    if (ind == data.end())
    {
        // Generate the split
        if (isClient)
        {
            data.insert({x, std::make_shared<_ChunkDataSplit>(x, loadPtr, loadDistance,
                                                              fd, isFdSet, chunkReady, shader)});
        }
        else
        {
            data.insert({x, std::make_shared<_ChunkDataSplit>(x, loadPtr, loadDistance,
                                                              chunkGen, fd, isFdSet, chunkReady, shader)});
        }
        
        auto ret = data.find(x);

        auto splitLeft = data.find(x - 1),
            splitRight = data.find(x + 1);

        if (splitLeft != data.end())
        {
            ret->second->left = splitLeft->second.get();
            splitLeft->second->right = ret->second.get();
        }
        
        if (splitRight != data.end())
        {
            ret->second->right = splitRight->second.get();
            splitRight->second->left = ret->second.get();
        }

        // Get chunks to generate
        ret->second->updateLoaded();
        
        return data.find(x);
    }
    return ind;
}
