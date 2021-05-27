#pragma once

#ifndef CHESS_PIECE_HXX
#define CHESS_PIECE_HXX

#endif //CHESS_PIECE_HXX

#include "player.hxx"

//Piece: a class based on a chess piece, which has
//both a piece type and a player attribute.
//has attribute "active" to determine whether or
//not piece is something that can be played or if inactive

enum class Piece_type {
    pawn;
    knight;
    rook;
    bishop;
    queen;
    king
};

class Piece{

public:
    Piece_type type;
    Player player;
    bool active;

    void remove_piece()
            {active = false;};
};