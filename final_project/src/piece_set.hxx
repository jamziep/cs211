#pragma once

#ifndef CHESS_PIECE_SET_HXX
#define CHESS_PIECE_SET_HXX

#endif //CHESS_PIECE_SET_HXX

#include "piece.hxx"
#include <ge211.hxx>

// #include <bitset>
// #include <cstddef>
// #include <initializer_list>

/// Originally, an efficient set of `ge211::Posn`s.
/// For chess, has been modified to be a set of class "Piece",
/// which contains a piece_type, a player, and a position.

/// The allowed range of the positions is limited by the constant
/// `coord_limit` below. Right now it's 8, which means that the largest
/// position a set can hold is {7, 7}.

class Piece_set
{
public:
    /// All position coordinates must be between 0 (inclusive) and
    /// `coord_limit` (exclusive).
    static constexpr int coord_limit = 8;

    //Constructor
    Piece_set(std::vector<Piece> pieces);

    //Gets the piece at a given position. Gets a reference
    //so you can modify it.
    Piece& get(Position);

    //Changes the position of a piece by reference.
    void set_posn(Piece&, Position);

    //changes the position of a piece located at a certain
    //position "start" to a new position "end"
    void change_posn(Position start, Position end);

    //Removes a piece from the piece set. Really does this by
    //setting "active" to false, so piece won't show up on board.
    void remove(Piece);

    //gets the size of a piece set
    size_t size();

    // Prints a `Piece_set`; suitable for debugging.
    // std::ostream&
    // operator<<(std::ostream&, Piece_set);

    // Returns whether position `p` is present in this set.
    //
    // ## Preconditions
    //
    //  - 0 <= p.x < coord_limit
    //
    //  - 0 <= p.y < coord_limit
    //
    // ## Example
    //
    // ```
    // if (set[pos]) {
    //     std::cout << "Position " << pos << " is in the set.\n";
    // }
    // ```
    bool operator[](Position posn) const;

    /// Are two position sets equal?
    friend bool operator==(Piece_set, Piece_set);

    /// Are two position sets unequal?
    friend bool operator!=(Piece_set, Piece_set);

private:

    ///Maximum number of possible positions.
    static constexpr std::size_t index_limit = coord_limit * coord_limit;

    //member data
    std::vector<Piece> pieces_;
};
