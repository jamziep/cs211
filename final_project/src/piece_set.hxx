#pragma once

#include "piece.hxx"

/// Originally, an efficient set of `ge211::Posn`s.
/// For chess, has been modified to be a set of class "Piece",
/// which contains a piece_type, a player, and a position.

/// The allowed range of the positions is limited by the constant
/// `coord_limit` below. Right now it's 8, which means that the largest
/// position a set can hold is {7, 7}.

class Piece_set
{
public:

    //Constructor
    Piece_set(std::vector<Piece> pieces);

    //Takes in a position and returns a copy of a piece in the set.
    Piece get_piece_from_set(Position) const;

    //Changes the position of a piece located at a certain
    //position "start" to a new position "end"
    void change_posn(Position start, Position end);

    //Removes a piece from the piece set. Really does this by
    //setting "active" to false, so piece won't show up on board.
    void remove(Piece);

    //gets the size of a piece set
    size_t size();

    /// Returns whether position `p` is present in this set.
    ///
    /// ## Preconditions
    ///
    ///  - 0 <= p.x < coord_limit
    ///
    ///  - 0 <= p.y < coord_limit
    ///
    /// ## Example
    ///
    /// ```
    /// if (set[pos]) {
    ///     std::cout << "Position " << pos << " is in the set.\n";
    /// }
    /// ```
    bool operator[](Position posn) const;

    //Lets you iterate through the member data of the piece set.
    Piece operator[](size_t ii);

    /// Are two piece sets equal?
    friend bool operator==(Piece_set, Piece_set);

    /// Are two piece sets unequal?
    friend bool operator!=(Piece_set, Piece_set);

    void
    change_piece_as(Position pos);

private:

    //The member data of the class: a vector of Piece.
    //Each piece contains a piece type, a player, and a position.
    std::vector<Piece> pieces_;

};

