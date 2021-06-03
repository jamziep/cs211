#pragma once

#include <iostream>

// A player or lack thereof.
// Three "non-real-player" categories were used for different
// purposes like setting up empty squares, determining a winner,
// or determining when the game has been cut off by a time limit.

enum class Player
{
    black,
    white,
    neither,
    null,
    out_of_time
};

Player other_player(Player);

std::ostream& operator<<(std::ostream&, Player);
