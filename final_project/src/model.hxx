#pragma once

#ifndef CHESS_MODEL_HXX
  #define CHESS_MODEL_HXX

//#include "piece.hxx"
#include "board.hxx"

//#include <ge211.hxx>

//#include <iostream>
//#include <vector>

// Represents the state of the game.
class Model
{
public:

    /// Model dimensions will use `int` coordinates, as board dimensions do.
    using Dimensions = Board::Dimensions;

    /// Model positions will use `int` coordinates, as board positions do.
    using Position = Board::Position;

    /// Model rectangles will use `int` coordinates, as board rectangles do.
    using Rectangle = Board::Rectangle;

    /// Constructs a model with `size` as both its width and height.
    ///
    /// ## Errors
    ///
    ///  - Throws `ge211::Client_logic_error` if `size` is less than 2
    ///    or greater than 8.
    explicit Model();

    /// Constructs a model with the given width and height.
    ///
    /// ## Errors
    ///
    ///  - Throws `ge211::Client_logic_error` if either dimension is less
    ///    than 2 or greater than 8.
    // Model();

    /// Returns a rectangle containing all the positions of the board.
    /// This can be used to iterate over the positions.
    Rectangle board() const;

    /// Returns whether the game is finished. This is true when neither
    /// player can move.
    bool is_game_over() const
    {return turn() == Player::neither;}

    /// Returns the current turn, or `Player::neither` if the game is
    /// over.
    Player turn() const
    {return turn_;}

    /// Returns the winner, or `Player::neither` if there is no winner
    /// (either because the game isn't over, or because it's a draw).
    Player winner() const
    {return winner_;}

    /// Returns the player at the given position, or `Player::neither` if
    /// the position is unoccupied.
    ///
    /// ## Errors
    ///
    ///  - Throws `ge211::Client_logic_error` if the position is out of
    ///    bounds.
    Piece operator[](Position) ;//const;

    /// Returns a pointer to the move that will result if the current
    /// player plays at the given position. If the current player cannot
    /// play at the given position, returns `nullptr`. (Also returns
    /// `nullptr` if the position is out of bounds.)
    ///
    /// Note that the returned pointer must be borrowed from `next_moves_`,
    /// not a pointer to a local variable defined within this function.
    ///
    Move const* find_move(Position) const;

    /// Attempts to play a move at the given position for the current
    /// player. If successful, advances the state of the game to the
    /// correct player or game over.
    ///
    /// ## Errors
    ///
    ///  - Throws `ge211::Client_logic_error` if the game is over.
    ///
    ///  - Throws `ge211::Client_logic_error` if the move is not currently
    ///    allowed for the current player.
    ///
    void play_move(Position, Position);

    //determines whether or not the model is currently at a state of check.
    //applies to any theoretical model so that we can look at the status of
    // either the current board or another board after a move has been made
    bool is_in_check(Player p);

    //determines whether the model is at a state of checkmate.
    //takes in a player, then carries out is_in_check for all possible
    //future states of the board after a move is made
    bool is_checkmate(Player p);


#ifdef CS211_TESTING
    // When this class is compiled for testing, members of a struct named
    // Test_access will be allowed to access private members of this class.
    friend struct Test_access;
#endif

private:
    //
    // PRIVATE MEMBER VARIABLES
    // (Don't change these!)
    //

    Player turn_   = Player::dark;
    Player winner_ = Player::neither;
    Board board_;

    Move_map next_moves_;
    std::vector<Piece> pieces_taken_;

    // INVARIANT:
    //  - `next_moves_` is always current for the state of the game.

    //helper function for compute_next_moves: finds spaces of travel
    //for pieces that can move an unlimited # of spaces: rook, bishop, queen
    Position_set spaces_ult(Piece);

    //helper function for compute_next_moves: finds spaces of travel
    //for pieces that only move a limited # of pieces: pawn, knight, king
    Position_set spaces_ltd(Piece);

    //adapted from reversi. takes in a position of a piece and a direction
    //of travel, and finds all the moves where the piece can go
    //in that direction, including positions where an enemy piece exists
    Position_set moves_in_dir_(Position, Dimensions);

    //takes in a position where the piece starts, a piece where it ends, and
    //changes the position of that piece
    void set_new_posn(Position start, Position end);

    /// Updates `next_moves_` to contain the moves available the current
    /// player.
    ///
    /// (Helper for `advance_turn_` and `Model(int, int)`.)
    void compute_next_moves_();

    /// Sets the turn to neither and determines the winner, if any.
    ///
    /// (Helper for `really_play_move_`.)
    void set_game_over_();

};

#endif //chess_model_hxx

