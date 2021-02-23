#pragma once

enum commands
{
    COMMAND_QUIT,
    COMMAND_PLAYER_POS,
    COMMAND_GET_CHUNK,
    COMMAND_GET_PLAYERS,
    COMMAND_PLACE_BLOCK_ABSOLUTE,
    COMMAND_DESTROY_BLOCK_ABSOLUTE
};

enum command_actions
{
    ACTION_ZERO,
    ACTION_ECHO,
    ACTION_GET_CHUNK,
    ACTION_PLACE_BLOCK,
    ACTION_DESTROY_BLOCK
};