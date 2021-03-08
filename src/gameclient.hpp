#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <thread>
#include <mutex>
#include <atomic>
#include <cstdint>
#include <random>
#include <cmath>
#include <ctime>
#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "graphics.hpp"
#include "api.hpp"
#include "server.hpp"
#include "clientsocket.hpp"
#include "winsize.hpp"
#include "eventwrapper.hpp"
#include "constants.hpp"
#include "texturehandler.hpp"
#include "camera.hpp"
#include "chunkgen.hpp"
#include "chunkgroup.hpp"
#include "player.hpp"
#include "animation.hpp"
#include "inventory.hpp"
#include "text.hpp"
#include "gravityparticle.hpp"
#include "tree.hpp"
#include "background.hpp"
#include "background_overworld.hpp"
#include "time.hpp"
#include "timer.hpp"
#include "player_group.hpp"
#include "instancedmodel.hpp"
#include "particlegroup.hpp"

struct SelectInfo
{
    int x;
    int y;
};

struct GameClient
{
    GameClient(const GLuint shader, Timer& timer, WinSize& winSize);
    GameClient(const GLuint shader, std::string address, uint16_t port, Timer& timer, WinSize& winSize);
    ~GameClient();

    void update(Camera& camera, EventWrapper& events);
    void render(Graphics& graphics, EventWrapper& events) const;
    bool exit;
private:
    void init();
    
    // Threading
    std::shared_ptr<Server> server;
    std::shared_ptr<ClientSocket> clientSocket;
    std::thread serverThread;
    std::mutex csLock;

    std::function<void(void)> disconnectCallback;
    
    WinSize& winSize;
    SelectInfo getSelection(Camera& camera, EventWrapper& events) const;
    void serverThreadFunction();
    void drawSelection(const Graphics& graphics, const SelectInfo pos) const;
    void handleCamera(Camera& camera);
    void mouseEvents(Camera& camera, EventWrapper& events);

    PlayerGroup serverPlayers;

    //std::unique_ptr<Inventory> inv;
    ChunkGroup serverChunks;

    // Entities
    Player mainPlayer;
    std::vector<std::shared_ptr<Entity>> entities;

    Animation fade;
    Animation playerPosUpdate;

    Time time;
    Timer& timer;

    std::shared_ptr<Background> background;

    ParticleGroup particleGroup;
    InstancedModel particleModel;
};
