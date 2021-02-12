#include "main.hpp"

int main()
{
    constexpr uint32_t DEFAULT_PORT = 8726;
    
    try
    {
        ServLog::log("Trying to initialize server...");
        Server server(DEFAULT_PORT);

        // Note: Blocks here
        server.startServer(1);
    }
    catch(const NetworkError& err)
    {
        // IDEA if port is below 1024 and type is SOCKET_ERROR, warn if not root
        ServLog::error(std::string{"Failed to start server: "} + std::string{err.what()});
        return err.type;
    }

    
    return 0;
}
