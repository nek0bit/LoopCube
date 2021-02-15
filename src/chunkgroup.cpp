#include "chunkgroup.hpp"

//***************************************
// _ChunkDataSplit: For vertical chunks
//***************************************
_ChunkDataSplit::_ChunkDataSplit(long int x, LoadPtr& loadPtr, LoadDistance& loadDistance,
                                 std::shared_ptr<ChunkGen> chunkGen)
    : loadPtr{loadPtr},
      loadDistance{loadDistance},
      loadedChunks{},
      left{nullptr},
      right{nullptr},
      x{x},
      chunkGen{chunkGen}
{
#ifndef __HEADLESS
    prepareLoaded();
#endif
}

// Generates a chunk if it isn't loaded
std::unordered_map<long int, ChunkData>::iterator _ChunkDataSplit::checkGenerate(long int y)
{
    auto ind = data.find(y);
    // Check if element doesn't exists
    if (ind == data.end())
    {
        // Generate the chunk
        data.insert({y, ChunkData{true, std::make_shared<Chunk>(Chunk{chunkGen, x, y})}});
        auto current = data.find(y);

        std::shared_ptr<Chunk>& newChunk = current->second.data;

        auto chunkAbove = data.find(y - 1),
            chunkBelow = data.find(y + 1);

        if (chunkAbove != data.end())
        {
            newChunk->borders.top = chunkAbove->second.data;
            newChunk->regenBlockBorders();
            chunkAbove->second.data->borders.bottom = newChunk;
            chunkAbove->second.data->regenBlockBorders();
        }
        
        if (chunkBelow != data.end())
        {
            newChunk->borders.bottom = chunkBelow->second.data;
            newChunk->regenBlockBorders();
            chunkBelow->second.data->borders.top = newChunk;
            chunkBelow->second.data->regenBlockBorders();
        }

        if (left != nullptr)
        {
            std::shared_ptr<Chunk> chunkLeft = left->getData(y);
            if (chunkLeft != nullptr)
            {
                chunkLeft->borders.right = newChunk;
                chunkLeft->regenBlockBorders();
                newChunk->borders.left = chunkLeft;
            }
        }

        if (right != nullptr)
        {
            std::shared_ptr<Chunk> chunkRight = right->getData(y);
            if (chunkRight != nullptr)
            {
                chunkRight->borders.left = newChunk;
                chunkRight->regenBlockBorders();
                newChunk->borders.right = chunkRight;
            }
        }
        
        return current;
    }
    return ind;
}

// Clears and reupdates the loadedChunks vector
void _ChunkDataSplit::updateLoaded()
{
    for (size_t i = 0; i < loadedChunks.size(); ++i)
    {
        const int chunkPos = -loadPtr.y + i;
        std::shared_ptr<Chunk> dataReceived = getData(chunkPos);
        
        // Generate if nullptr (might want to remove this later)
        if (dataReceived == nullptr)
        {            
            dataReceived = checkGenerate(chunkPos)->second.data;
        }
        
        loadedChunks[i] = dataReceived;
    }
}

// Update all loaded chunks in the split
#ifndef __HEADLESS
void _ChunkDataSplit::updateSplit(Camera& camera)
{
    // TODO Remove me, just for debugging
    updateLoaded();
    
    for (auto& chunk: loadedChunks)
    {
        chunk->updateAll(camera);
    }
}

// Renders all loaded chunks in the split if visible
void _ChunkDataSplit::renderSplit(SDL_Renderer* renderer, TextureHandler& textures, Camera& camera)
{
    for (auto& chunk: loadedChunks)
    {
        //chunk->renderAllShadows(renderer, camera);
        chunk->renderAllBlocks(renderer, textures, camera);
    }
}
#endif

std::shared_ptr<Chunk> _ChunkDataSplit::getData(long int y)
{
    try { return data.at(y).data; }
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

ChunkGroup::ChunkGroup(std::shared_ptr<ChunkGen> chunkGen)
    :  loadPtr{-5, -5},
       loadDistance{constants::loadDistance},
       chunkGen{chunkGen},
       loadedSplits{},
       data{}
{
    // Generate vertical splits
#ifndef __HEADLESS
    prepareLoaded();
    updateLoaded();
#endif
}
      
void ChunkGroup::updateLoaded()
{
    for (size_t i = 0; i < loadedSplits.size(); ++i)
    {
        const int chunkPos = loadPtr.x + i;
        std::shared_ptr<_ChunkDataSplit> dataReceived = getData(chunkPos);
        
        // Generate if nullptr (might want to remove this later)
        if (dataReceived == nullptr)
        {            
            dataReceived = checkSplitGenerate(chunkPos)->second;
        }
        
        loadedSplits[i] = dataReceived;
    }    
}

void ChunkGroup::prepareLoaded()
{
    loadedSplits.clear();
    loadedSplits.resize(loadDistance.x);
}

// Cleaner way to get at data
std::shared_ptr<_ChunkDataSplit> ChunkGroup::getData(long int x)
{
    try { return data.at(x); }
    catch (const std::out_of_range& err) { return nullptr; }
}

#ifndef __HEADLESS
void ChunkGroup::update(Camera& camera)
{
    // TODO Remove me, just for testing things out
    updateLoaded();

    for (auto& split: loadedSplits)
    {
        if (split == nullptr) continue;
        split->updateSplit(camera);
    }
}

void ChunkGroup::render(SDL_Renderer* renderer, TextureHandler& textures, Camera& camera)
{
    for (auto& split: loadedSplits)
    {
        if (split == nullptr) continue;
        split->renderSplit(renderer, textures, camera);
    }
}
#endif

// Wrapper for multiple getData calls
std::shared_ptr<Chunk> ChunkGroup::getChunkAt(const long int x, const long int y)
{
    std::shared_ptr<_ChunkDataSplit> call = getData(x);
    
    return call == nullptr ? nullptr : getData(x)->getData(y);
}

std::vector<std::shared_ptr<Chunk>> ChunkGroup::isWithinChunks(const Vec2& vec, const Size& size)
{
    ChunkPos inChunk = posToChunkPos(vec.x, vec.y);
    GridCollision_t col = Generic::gridCollision(constants::chunkWidth * constants::blockW,
                                                 constants::chunkHeight * constants::blockH,
                                                 vec, size);

    std::vector<std::shared_ptr<Chunk>> ch{};

    for (size_t i = 0; i < col.width + 1; ++i)
    {
        for (size_t j = 0; j < col.height + 1; ++j)
        {
            ch.emplace_back(getChunkAt(inChunk.x + i, inChunk.y + j));
        }
    }

    return ch;
}

void ChunkGroup::generateChunkAt(const long x, const long y)
{
    auto splitX = checkSplitGenerate(x);
    splitX->second->checkGenerate(y);
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
        // Generate the chunk
        data.insert({x, std::make_shared<_ChunkDataSplit>(x, loadPtr, loadDistance, chunkGen)});
        auto ret = data.find(x);

        auto splitLeft = data.find(x - 1),
            splitRight = data.find(x + 1);

        if (splitLeft != data.end())
        {
            ret->second->left = splitLeft->second;
            splitLeft->second->right = ret->second;
        }
        
        if (splitRight != data.end())
        {
            ret->second->right = splitRight->second;
            splitRight->second->left = ret->second;
        }

        // Get chunks to generate
        ret->second->updateLoaded();
        
        return data.find(x);
    }
    return ind;
}
