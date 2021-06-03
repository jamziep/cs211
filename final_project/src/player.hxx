#pragma once

#include <iostream>

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
