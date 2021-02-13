#include "main.hpp"

void sigQuit(int signum)
{
    std::cout << " - Caught Signal " << signum << "!" << std::endl;
    
    // Calls destructor and deletes it
    delete serverPtr;
    serverPtr = nullptr;

    // Bye
    exit(0);
}

int main()
{
    constexpr uint32_t DEFAULT_PORT = 8726;

    signal(SIGINT, sigQuit);
    
    try
    {
        ServLog::log("Trying to initialize server...");
        serverPtr = new Server(DEFAULT_PORT);

        // Note: Blocks here
        serverPtr->startServer(4);
    }
    catch(const NetworkError& err)
    {
        // IDEA if port is below 1024 and type is SOCKET_ERROR, warn if not root
        ServLog::error(std::string{"Failed to start server: "} + std::string{err.what()});
        delete serverPtr;
        return err.type;
    }
    catch(const std::exception& err)
    {
        ServLog::error(err.what());
        delete serverPtr;
        return -2;
    }

    delete serverPtr;
    return 0;
}
