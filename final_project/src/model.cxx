#include "model.hxx"

//for debug
#include <iostream>

using namespace ge211;


Model::Model()
        : board_({8,8}),
          next_moves_(),
          pieces_taken_()
{

    //initialize next_moves_ to turn_'s possible next moves,
    //using the compute_next_moves helper
    //Model::compute_next_moves_();
}

Player Model::turn() const
{return turn_;}

Player Model::winner() const
{return winner_;}

bool Model::is_game_over() const
{return turn() == Player::neither;}


Model::Rectangle Model::board() const
{
    return board_.all_positions();
}

Piece Model::operator[](Position pos) //const
{
    return board_[pos];
}

Move const* Model::find_move(Position pos) const
{
    auto i = next_moves_.find(pos);

    if (i == next_moves_.end())
        return nullptr;
    else
        return &*i;
}

void Model::play_move(Position pos)
{
    if (is_game_over())
        //rather than throw an error, run our own game over function
        set_game_over_();

    Move const* movep = find_move(pos);
    if (!movep) {
        //now that we've implemented checks for this in controller, we actually
        //do want this error to throw if we get here, b/c it means we did some-
        //thing wrong
        throw Client_logic_error("Model::play_move: no such move");
    } else {

        //find the position set of all things we've changed via find_move()
        // Position_set pset = movep->second;
        Player turn = Model::turn();

        //add the position set of all the things we've changed via find_move()
        //to the data of which tiles are where in board
        //Model::board_.set_all(pset, turn);

        //advance the turn. using function from player.cxx
        Model::turn_ = other_player(Model::turn_);

        //refill next_moves_ for the current player
        Model::compute_next_moves_();
        if (next_moves_.empty()){
            Model::turn_ = other_player(Model::turn_);

            //if neither player has any moves left, set game over
            Model::compute_next_moves_();
            if(next_moves_.empty()){
                set_game_over_();
            }
        }
    }
}

//
// BELOW ARE *OPTIONAL* HELPER FUNCTIONS
//

Position_set Model::evaluate_position_(Position pos) const
{
    //try initializing the position set with the posn we're looking at,
    //as well, so it gets flipped too
    Position_set curr_pos {};

    // iterate through all possible directions and call find_flips
    for (auto dir : Board::all_directions())
    {
        //find the next square in each direction.
    }

    //if the curr_pos is not empty, meaning this is a valid move that will cause
    //flips, add curr_pos to the Position_set that will become "second" in
    // next_moves
    if (curr_pos.size()) {
        Position_set start_posn{pos};
        curr_pos |= start_posn;
    }

    return curr_pos;
}

Position_set Model::moves_in_dir_(Position current, Dimensions dir) {

    //taken from reversi. modified to take into account chess logic
    Position_set moves_in_dir {};
    std::size_t n = 1;

    //perform bounds_checking on the next square in the board in the given
    //direction. if first adjacent square would be out of bounds, do nothing
    Position posn = current + n * dir;
    if (posn.x < 0 || posn.x >= Model::board_.dimensions().width
        || posn.y < 0 || posn.y >= Model::board_.dimensions().height) {
        return moves_in_dir;
    }

    // increments n when we detect the space is occupied by a tile of the
    // other player
    while (board_[posn].get_piece_type() == Piece_type::null) {
        n++;

        //perform bounds checking. once we reach an out-of-bounds posn,
        //stop adding to moves_in_dir
        posn = current + n * dir;
        if (posn.x < 0 || posn.x >= Model::board_.dimensions().width
            || posn.y < 0 || posn.y >= Model::board_.dimensions().height) {

            //decrement n so that model only looks at the max valid index
            // in next part of code
            n--;
            break;
        }
    }

    // if the final tile is the opposite player, then we add the
    // positions up to and including that one to the set of moves_in_dir.
    if (board_[current + n * dir].get_player() == other_player(turn_))
    {
        for(std::size_t ii = 1; ii < n; ii++){
            moves_in_dir[{current + ii * dir}] = true;
        }

    //else, if the final tile is the same player, do the same as above
    //but subtract 1 from n
    } else {
        n--;
        for(std::size_t ii = 1; ii < n; ii++){
            moves_in_dir[{current + ii * dir}] = true;
        }

    }

    //return all the valid moves for a piece in this dir
    return moves_in_dir;
}



//calculate possible moves for pieces that can move unlimited # of spaces
//includes rook, bishop, queen
Position_set Model::spaces_ult(Piece p)
{
    //initialize sets of data of unknown size
    Position_set possible_moves = Position_set();
    std::vector<Board::Dimensions> dirs_travel;

    //find the directions of travel possible for this piece
    switch(p.get_piece_type()) {

    case Piece_type::rook:
        dirs_travel = board_.rook_directions();
    case Piece_type::bishop:
        dirs_travel = board_.bishop_directions();
    case Piece_type::queen:
        dirs_travel = board_.all_directions();
    default:
        throw Client_logic_error("Model::spaces_ult: piece must be r/b/q");
    }

    //in each direction of travel, take the current posn of piece
    //and add the vector of the dir_travel to it.
    // - bounds-check the posn to make sure it's between (0,0) and (7,7)
    //      --this happens in [], but could throw an error
    // - move as many free spaces as possible in that direction
    // - if there's a piece at the end of the line, check if it's an enemy
    // - piece, in which case we'd be able to take that piece
    for (auto dir : dirs_travel) {

        //add the moves possible in this direction to the total list
        //of possible moves for this piece using union |=
        Position_set moves_in_dir = moves_in_dir_(p.get_posn(), dir);
        possible_moves.operator|=(moves_in_dir);
    }

    //return all possible moves for this piece
    return possible_moves;
}

//calculate possible moves for pieces that can move a limited # of spaces
//includes pawn, knight, king
Position_set Model::spaces_ltd(Piece p)
{


}



void Model::compute_next_moves_()
{
    // iterate through entire board all call eval_position on every spot.
    // Only add non empty position sets  to next_moves_.
    next_moves_.clear(); // first clear out next moves

    //iterate through each

}

void Model::set_game_over_()
{
    Model::turn_ = Player::neither;

}


//is_check

//is_checkmate

