#include "chunkgroup.hpp"

//***************************************
// _ChunkDataSplit: For vertical chunks
//***************************************
_ChunkDataSplit::_ChunkDataSplit(long int x, LoadPtr& loadPtr, LoadDistance& loadDistance)
    : loadPtr{loadPtr},
      loadDistance{loadDistance},
      x{x}
{
    prepareLoaded();
}

// Generates a chunk if it isn't loaded
std::unordered_map<long int, ChunkData>::iterator _ChunkDataSplit::checkGenerate(long int y)
{
    auto ind = data.find(y);
    // Check if element doesn't exists
    if (ind == data.end())
    {
        // Generate the chunk
        data.insert({y, ChunkData{true, std::make_shared<Chunk>(Chunk{x, y})}});
        return data.find(y);
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
        chunk->renderAllShadows(renderer, camera);
        chunk->renderAllBlocks(renderer, textures, camera);
    }
}

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

ChunkGroup::ChunkGroup()
    : loadedSplits{},
      data{},
      loadPtr{-5, -5},
      loadDistance{constants::loadDistance}
{
    // Generate vertical splits
    prepareLoaded();
    updateLoaded();
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

// Wrapper for multiple getData calls
std::shared_ptr<Chunk> ChunkGroup::getChunkAt(const long int x, const long int y)
{
    std::shared_ptr<_ChunkDataSplit> call = getData(x);
    
    return call == nullptr ? nullptr : getData(x)->getData(y);
}

ChunkPos ChunkGroup::posToChunkPos(const double x, const double y) const
{
    return ChunkPos{static_cast<long>(x / (constants::chunkWidth * constants::blockW)),
        static_cast<long>(y / (constants::chunkHeight * constants::blockH))};
}

std::unordered_map<long int, std::shared_ptr<_ChunkDataSplit>>::iterator
ChunkGroup::checkSplitGenerate(long int x)
{
    auto ind = data.find(x);
    // Check if element doesn't exists
    if (ind == data.end())
    {
        // Generate the chunk
        data.insert({x, std::make_shared<_ChunkDataSplit>(x, loadPtr, loadDistance)});
        auto ret = data.find(x);
        ret->second->updateLoaded();
        return data.find(x);
    }
    return ind;
}
