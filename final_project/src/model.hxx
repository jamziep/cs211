#pragma once

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
    void play_move(Position, Position);

    bool is_in_check(Player p) const;

    bool no_moves_left(Player p) const;

    Position find_king(Player p) const
    {return board_.find_king_location(p);}

    //lets the view communicate with the model and set "out of time"
    void set_out_of_time(int time_limit);

private:

    Player turn_   = Player::white;
    Player winner_ = Player::neither;
    Board board_;

    Move_map next_moves_;

    ge211::Pausable_timer black_timer;
    ge211::Pausable_timer white_timer;

    bool valid_moves_white;
    bool valid_moves_black;

    Position_set spaces_ult(Piece);

    Position_set spaces_ltd(Piece);

    Position_set moves_in_dir_(Position, Dimensions);

    void set_new_posn(Position start, Position end);

    /// Updates `next_moves_` to contain the moves available the current
    /// player.
    ///
    /// (Helper for `advance_turn_` and `Model(int, int)`.)
    void compute_next_moves_();

    void modify_next_moves_();

public:

    ge211::Duration elapsed_time_black()
    {return black_timer.elapsed_time();}

    ge211::Duration elapsed_time_white()
    {return white_timer.elapsed_time();}

    void pause_black()
    {black_timer.pause();}

    void pause_white()
    {white_timer.pause();}

    void resume_black()
    {black_timer.resume();}

    void resume_white()
    {white_timer.resume();}

    void castle_check(Position, Position);
    bool Rrook_castle (Player plr);
    bool Lrook_castle (Player plr);

    void p_promo(Position pos);
    Piece_type
    return_piece_type(Position posn);
};

