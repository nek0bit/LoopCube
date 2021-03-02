#include "constants.hpp"

using namespace std::string_literals;

// Texture stuff
namespace constants
{
#if defined(__WIIU__) || defined(__SWITCH__)
	LoadDistance loadDistance{12, 12};
#else
	LoadDistance loadDistance{16, 16};
#endif

	const std::string header = "LoopCube";
	const std::vector<std::string> content = {
		"Welcome to LoopCube!\n",
		"Current Version: "s + versionStr + "\n",
        "\"Stable\" Multiplayer Edition\n"
	};

#ifndef __HEADLESS
    Config config{};
    FontHandler fontHandler{};
#endif
}
