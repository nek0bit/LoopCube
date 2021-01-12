#pragma once

#include <string>

struct BlockInfo
{
	int id; // Identifier
	std::string displayName;
	int textureId;
	bool noCollision;
    
	// Implementable stuff
	//bool can_fall; // These blocks start in a falling state until collision, then they become normal
	// If a block around them updates, check if below, then fall again
	//int destroy_time; // -1 would mean that it can't be destroyed at all
	//bool has_use; // There is a future to add function where the player can use, default = no
	//bool literally_can_fall; // Block has permanent gravity, I guess it would be cool to play with; used for overriding can_fall	
};
