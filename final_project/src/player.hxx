#pragma once

#ifndef FINAL_PROJECT_PLAYER_HXX
  #define FINAL_PROJECT_PLAYER_HXX

#endif //FINAL_PROJECT_PLAYER_HXX


#include <iostream>

// A player or lack thereof.
enum class Player
{
    dark,
    light,
    neither,
};

// Returns the other player.
Player other_player(Player);

// Prints a player in a manner suitable for debugging.
std::ostream& operator<<(std::ostream&, Player);
