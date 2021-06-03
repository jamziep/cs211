#pragma once

#include "move.hxx"
#include "piece_set.hxx"
// #include "std_black.hxx"
// #include "std_white.hxx"

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

/// Represents the state of the board.
class Board
{
public:
    //
    // HELPFUL TYPE ALIASES
    //

    /// Board dimensions will use `int` coordinates.
    using Dimensions = ge211::Dims<int>;

    /// Board positions will use `int` coordinates.
    using Position = ge211::Posn<int>;

    /// Board rectangles will use `int` coordinates.
    using Rectangle = ge211::Rect<int>;

private:

    Dimensions dims_;

    //Two vectors of type Piece to represent the chess pieces.
    Piece_set white_;
    Piece_set black_;

    // INVARIANT: (light_ & dark_) at initial chess board state

public:
    //
    // PUBLIC CONSTRUCTOR & FUNCTION MEMBERS
    //

    /// Constructs a board with the given dimensions.
    ///
    /// ## Errors
    ///
    ///  - Throws `ge211::Client_logic_error` if either dimension is less
    ///    than 2 or greater than 8.
    explicit Board(Dimensions dims);

    /// Returns the same `Dimensions` value passed to the
    /// constructor.
    Dimensions dimensions() const;

    /// Returns whether the given position is in bounds.
    bool good_position(Position) const;

    /// Returns the `Player` stored at `pos`.
    ///
    /// ## Errors
    ///
    ///  - throws `ge211::Client_logic_error` if `!good_position(pos)`.
    Piece operator[](Position pos) const;

    ///Sets the piece type on board by modifying the appropriate Piece_set.
    ///Used primarily for pawn promotion to change type to "queen".
    void set_piece_as(Piece piece);

    //
    // PUBLIC CONSTRUCTOR & FUNCTION MEMBERS
    //

    /// Returns a rectangle containing all the positions of the board. This
    /// can be used to iterate over the positions:
    ///
    /// ```
    /// for (Position pos : a_board.all_positions()) {
    ///     ... a_board[pos] ...;
    /// }
    /// ```
    ///
    /// Note that `Rectangle`s are considered to be closed on the top
    /// and left, but open on the bottom and right. The iterator will visit
    /// the correct positions for the board.
    Rectangle all_positions() const;

    /// Returns a reference to a `std::vector` containing all eight "unit
    /// direction vectors". In Python notation, these are:
    ///
    /// ```python
    /// { Dims(dx, dy)
    ///       for dx in [-1, 0, 1]
    ///           for dy in [-1, 0, 1]
    ///               if dx or dy }
    /// ```
    static std::vector<Dimensions> const& all_directions();

    //directions of travel for knight
    static std::vector<Dimensions> const& knight_directions();

    //directions of travel for bishop
    static std::vector<Dimensions> const& bishop_directions();

    //directions of travel for rook
    static std::vector<Dimensions> const& rook_directions();

    //directions of travel for pawn. Modify_pawn_dirs() takes in the basic
    //info on all directions a pawn can move, and sees if these are feasible
    //based on the pieces around the pawn.
    static std::vector<Dimensions> const& pawn_directions_light();
    static std::vector<Dimensions> const& pawn_directions_dark();
    std::vector<Dimensions> modify_pawn_dirs(Piece, std::vector<Dimensions>);

    //take in a piece, and change its position. essential for making
    //a move
    void change_piece_posn(Piece p, Position posn);

    //find the location of the king in either of the piece_sets
    Position find_king_location(Player) const;

    //remove a piece from board by setting it to "active = false". search
    //by position because that's more convenient
    void remove_by_posn(Position);

    /// Equality for boards.
    friend bool operator==(Board const&, Board const&);

private:
    //
    // PRIVATE HELPER FUNCTION MEMBERS
    //

    Piece get_piece_(Position where) const;
    void bounds_check_(Position where) const;

#ifdef CS211_TESTING
    // When this class is compiled for testing, members of a struct named
    // Test_access will be allowed to access private members of this class.
    friend struct Test_access;
#endif
};


//
// FREE FUNCTIONS FOR WORKING WITH THE CLASS
//

/// Inequality for boards.
bool
operator!=(Board const&, Board const&);

/// Board printing, suitable for debugging.
std::ostream&
operator<<(std::ostream&, Board const&);


