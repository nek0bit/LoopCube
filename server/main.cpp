#include "main.hpp"

int main()
{
    std::cout << "[server] Starting..." << std::endl;

    Server server("127.0.0.1", 23);

    // Note: Blocks here
    server.startServer();
    
    return 0;
}
