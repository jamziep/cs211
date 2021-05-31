
#include "player.hxx"

Player other_player(Player player)
{
    switch (player) {
    case Player::white: return Player::black;
    case Player::black: return Player::white;
    default: return player;
    }
}

std::ostream& operator<<(std::ostream& os, Player p)
{
    switch (p) {
    case Player::white:
        return os << "W";
    case Player::black:
        return os << "B";
    default:
        return os << "_";
    }
}

