#pragma once

#include "player.hxx"
#include "position_set.hxx"

using Position = ge211::Posn<int>;

///Piece: a class based on a chess piece, which has
///both a piece type and a player attribute.
///has attribute "active" to determine whether or
///not piece is something that can be played or if inactive

enum class Piece_type {
    pawn,
    knight,
    rook,
    bishop,
    queen,
    king,
    null
};

class Piece{

public:

    Piece(Piece_type ptype, Player aplayer, Position aposn);

    Player get_player()
    {return player_;}

    Piece_type get_piece_type()
    {return type_;}

    Position get_posn()
    {return posn_;}

    bool is_active()
    {return active_;}

    void set_posn(Position posn)
    {posn_ = posn;}

    void remove_piece()
            {active_ = false;};

    void promote_piece()
        {type_ = Piece_type::queen;};

    ///two pieces are equal if their member data is the same
    friend bool operator==(Piece, Piece);

    friend bool operator!=(Piece, Piece);

    /// for setting a certain piece as a certain.

private:
    Piece_type type_;
    Position posn_;
    Player player_;
    bool active_;

};