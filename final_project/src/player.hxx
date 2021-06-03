#pragma once

#include <iostream>

// A player or lack thereof.
enum class Player
{
    black,
    white,
    neither,
    null,
    out_of_time
};

// Returns the other player.
Player other_player(Player);

// Prints a player in a manner suitable for debugging.
std::ostream& operator<<(std::ostream&, Player);
