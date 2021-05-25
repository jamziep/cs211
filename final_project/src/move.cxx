//
// Created by seanp on 5/25/2021.
//

#include "move.hxx"

std::ostream&
operator<<(std::ostream& os, Move const& move)
{
    return os
            << "Move{{" << move.first.x << ", " << move.first.y << "}, "
            << move.second << "}";
}
