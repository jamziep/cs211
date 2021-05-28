#include "model.hxx"

//for debug
#include <iostream>

using namespace ge211;

Model::Model(int size)
        : Model(size, size)
{ }

Model::Model(int width, int height)
        : board_({width, height}),
          next_moves_(),
          pieces_taken_()
{
    //for now set up the board with all the squares in the
    //basic positions
    //board_[{0,0}] = Piece(Piece_type::rook,Player::dark,Position{0,0});



    //initialize next_moves_ to turn_'s possible next moves,
    //using the compute_next_moves helper
    Model::compute_next_moves_();
}


Model::Rectangle Model::board() const
{
    return board_.all_positions();
}

Piece Model::operator[](Position pos) const
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

void Model::compute_next_moves_()
{
    // iterate through entire board all call eval_position on every spot.
    // Only add non empty position sets  to next_moves_.
    next_moves_.clear(); // first clear out next moves
    // int total_plays = board_.count_player(Player::light) + board_
    //         .count_player(Player::dark);

}


void Model::set_game_over_()
{
    Model::turn_ = Player::neither;

}


//is_check

//is_checkmate

