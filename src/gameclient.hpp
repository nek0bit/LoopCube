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
#include "testentity.hpp"
#include "animation.hpp"
#include "inventory.hpp"
#include "text.hpp"
#include "gravityparticle.hpp"
#include "tree.hpp"
#include "background.hpp"
#include "background_overworld.hpp"
#include "time.hpp"
#include "timer.hpp"

struct SelectInfo
{
    int x;
    int y;
};

struct GameClient
{
    GameClient(Timer& timer, WinSize& winSize);
    GameClient(std::string address, uint16_t port, Timer& timer, WinSize& winSize);
    ~GameClient();

    void update(EventWrapper& events);
    void render(SDL_Renderer* renderer, TextureHandler& textures, EventWrapper& events);
private:
    // Threading
    std::shared_ptr<Server> server;
    std::shared_ptr<ClientSocket> clientSocket;
    std::thread serverThread;
    std::mutex csLock;
    
    WinSize& winSize;
    SelectInfo getSelection(EventWrapper& events);
    void serverThreadFunction();
    void drawSelection(SDL_Renderer* renderer, const SelectInfo pos);
    void handleCamera();
    void deadParticles();
    void mouseEvents(EventWrapper& events);

    //std::unique_ptr<Inventory> inv;
    ChunkGroup serverChunks;
    Camera camera;

    // Entities
    Player mainPlayer;
    std::vector<std::shared_ptr<Entity>> entities;

    Animation fade;

    std::vector<GravityParticle> particles;

    Time time;
    Timer& timer;
};
