#include "player_group.hpp"

PlayerGroup::PlayerGroup()
{

}

PlayerGroup::~PlayerGroup()
{

}

void PlayerGroup::updatePlayers(ChunkGroup& chunks, Timer& timer,
                                std::vector<std::shared_ptr<Entity>>& entities)
{
    // Loop through each player and update them
    for (auto& p: players)
    {
        p.second.update(chunks, timer, entities, true);
    }
}

void PlayerGroup::renderPlayers(const Graphics& renderer, TextureHandler& textures,
                                const Camera& camera) const
{
    // Render players
    for (auto& p: players)
    {
        p.second.render(renderer, camera);
    }
}

void PlayerGroup::updateSpecificPlayer(const int id, const int x, const int y)
{
    auto found = players.find(id);

    if (found != players.end() &&
        found->first == id)
    {
        Player& current = found->second;

        current.position.x = x;
        current.position.y = y;
    }
    else
    {
        addPlayer(id);
    }
}

void PlayerGroup::addPlayer(const int id)
{
    players.insert({id, {}});
}
