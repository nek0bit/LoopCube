#pragma once

// Messages
constexpr const char* _COMMAND_SUCCESS = "ACK"; // This shouldn't be sent very often!
constexpr const char* _COMMAND_FAIL = "NACK";

constexpr const char* COMMAND_QUIT = "QUIT";
constexpr const char* COMMAND_ECHO = "ECHO";
constexpr const char* COMMAND_PLAYER_POS = "PPOS";
constexpr const char* COMMAND_GET_CHUNK = "GC";
constexpr const char* COMMAND_GET_PLAYERS = "GP";
constexpr const char* COMMAND_PLACE_BLOCK_ABSOLUTE = "PBA";
constexpr const char* COMMAND_DESTROY_BLOCK_ABSOLUTE = "DBA";

enum command_actions
{
    ACTION_ZERO,
    ACTION_ECHO,
    ACTION_GET_CHUNK,
    ACTION_PLACE_BLOCK,
    ACTION_DESTROY_BLOCK
};
