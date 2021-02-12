#include <iostream>
#include <cstring>
#include <csignal>
#include <string>
#include "../src/server.hpp"

Server* serverPtr = nullptr;

void sigQuit(int signum);
