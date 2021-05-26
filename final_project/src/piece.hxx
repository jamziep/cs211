#pragma once

#ifndef CHESS_PIECE_HXX
#define CHESS_PIECE_HXX

#endif //CHESS_PIECE_HXX

#include "player.hxx"

using Position = ge211::Posn<int>;

//Piece: a class based on a chess piece, which has
//both a piece type and a player attribute.
//has attribute "active" to determine whether or
//not piece is something that can be played or if inactive

enum class Piece_type {
    pawn,
    knight,
    rook,
    bishop,
    queen,
    king
};

class Piece{

public:
    Piece(Piece_type ptype, Player aplayer);

    Piece_type type;
    Player player;
    bool active;

    void remove_piece()
            {active = false;};
};

std::ostream& operator<<(std::ostream&, Piece);

//a new type to enable us to represent a piece with a
//position and the type of piece

class Piece_set
{
public:

    //take inspiration from position_set for some of the operator== stuff

private:
    Position posn_;
    Piece piece_;

};