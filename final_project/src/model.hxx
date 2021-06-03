#pragma once

#ifndef CHESS_MODEL_HXX
  #define CHESS_MODEL_HXX

#include "board.hxx"

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
    explicit Model(bool use_stalemate);

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
    Piece operator[](Position) const;

    /// Returns a pointer to the move that will result if the current
    /// player plays at the given position. If the current player cannot
    /// play at the given position, returns `nullptr`. (Also returns
    /// `nullptr` if the position is out of bounds.)
    ///
    /// Note that the returned pointer must be borrowed from `next_moves_`,
    /// not a pointer to a local variable defined within this function.
    ///
    Move const* find_move(Position) const;

    /// Sees if there is a piece at the position where player wants to play, and
    /// checks to see that the position the player wants to move their piece to
    /// exists. i.e. check that "end" is in movep -> second, then sets the piece
    ///
    /// If there was already a piece at the place where the
    /// move was going to be made, other piece is removed from board.
    ///
    /// Other special game logic:
    /// - Check if the next player can castle
    /// - Pawn promotion check. Pawn auto-promotes to queen if the pawn is moved
    ///      to an end position in the opponent's back rank.
    /// - Pause the timer for the current player, and start the timer
    ///      for the other player
    ///
    /// Checks to see if we're at check or checkmate
    /// Changes the turn to opposite player and refills next_moves_

    /// ## Errors
    ///
    ///  - Throws `ge211::Client_logic_error` if the position the player
    ///     selected is invalid.
    ///
    ///  - Throws `ge211::Client_logic_error` if the move is not currently
    ///    allowed for the current player.
    ///
    void play_move(Position, Position, bool check4check);

    //determines whether or not the model is currently at a state of check.
    //applies to any theoretical model so that we can look at the status of
    // either the current board or another board after a move has been made
    bool is_in_check(Player p) const;

    //determines whether this player is at a state of checkmate.
    //takes in a player, then carries out is_in_check for all possible
    //future states of the board after a move is made
    bool no_moves_left(Player p) const;

    //finds the king for a player within its board member
    Position find_king(Player p) const
    {return board_.find_king_location(p);}


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

    Player turn_   = Player::white;
    Player winner_ = Player::neither;
    Board board_;

    Move_map next_moves_;

    //for timing in view.cxx
    ge211::Pausable_timer black_timer;
    ge211::Pausable_timer white_timer;

    bool valid_moves_white;
    bool valid_moves_black;

    //for deciding if board should calculate for stalemate
    bool use_stalemate;

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

    //Further updates next_moves_ to remove any moves that would
    //put the king in check (these are invalid moves)
    void modify_next_moves_();

public:

    //get data out of the timers
    ge211::Duration elapsed_time_black()
    {return black_timer.elapsed_time();}

    ge211::Duration elapsed_time_white()
    {return white_timer.elapsed_time();}

    //pause or start clock
    void pause_black()
    {black_timer.pause();}

    void pause_white()
    {white_timer.pause();}

    void resume_black()
    {black_timer.resume();}

    void resume_white()
    {white_timer.resume();}

    // helpers for castling
    void castle_check(Position, Position);
    bool Rrook_castle (Player plr);
    bool Lrook_castle (Player plr);

    // for pawn promotion
    void p_promo(Position pos);
    Piece_type
    return_piece_type(Position posn);
};

#endif //chess_model_hxx

