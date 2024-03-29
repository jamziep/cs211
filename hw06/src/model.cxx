 #include "model.hxx"

//for debug
#include <iostream>

using namespace ge211;

Model::Model(int size)
        : Model(size, size)
{ }

Model::Model(int width, int height)
        : board_({width, height}),
        next_moves_()
{
    //initialize next_moves_ to turn_'s possible next moves,
    //using the compute_next_moves helper
    Model::compute_next_moves_();

}

Model::Rectangle Model::board() const
{
    return board_.all_positions();
}

Player Model::operator[](Position pos) const
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
        Position_set pset = movep->second;
        Player turn = Model::turn();

        //add the position set of all the things we've changed via find_move()
        //to the data of which tiles are where in board
        Model::board_.set_all(pset, turn);

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

Position_set Model::find_flips_(Position current, Dimensions dir) const
{
    Position_set flips {};
    std::size_t n = 1;

    //perform bounds_checking on the next square in the board in the given
    //direction. if first adjacent square would be out of bounds, do nothing
    Position posn = current + n * dir;
    if (posn.x < 0 || posn.x >= Model::board_.dimensions().width
    || posn.y < 0 || posn.y >= Model::board_.dimensions().height) {
        return flips;
    }

    // increments n when we detect the space is occupied by a tile of the
    // other player
    while (board_[posn] == other_player(turn_)){
        n++;

        //perform bounds checking. once we reach an out-of-bounds posn,
        //stop adding to flips
        posn = current + n * dir;
        if (posn.x < 0 || posn.x >= Model::board_.dimensions().width
            || posn.y < 0 || posn.y >= Model::board_.dimensions().height) {

            //decrement n so that model only looks at the max valid index
            // in next part of code
            n--;
            break;
        }
    }

    if (board_[current + n * dir] == turn_)
    {
        // if the final tile is the current player, then we add the
        // positions up to that one to the set of flips.
        for(std::size_t ii = 1; ii < n; ii++){
            flips[{current + ii * dir}] = true;
        }
    }

    //add the current player to flips
    return flips;
}

Position_set Model::evaluate_position_(Position pos) const
{
    //try initializing the position set with the posn we're looking at,
    //as well, so it gets flipped too
    Position_set curr_pos {};

    // iterate through all possible directions and call find_flips
    for (auto dir : Board::all_directions())
    {
        //find the next square in each direction.
        Position next_pos = pos + dir;

        //filter out the directions that are out of bounds. Out of bounds
        //means a value less than 0 or greater than width of board
        if (next_pos.x < 0 || next_pos.x >= Model::board_.dimensions().width
        || next_pos.y < 0 || next_pos.y >= Model::board_.dimensions().height){
            continue;
        }

        Position_set new_tiles = find_flips_(pos, dir);
        curr_pos.operator|=(new_tiles); // update the curr_pos with flips
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
    int total_plays = board_.count_player(Player::light) + board_
            .count_player(Player::dark);

    // check to see if the 4 center moves have been played
    if (total_plays < 4){
        Rectangle center_4 = Model::board_.center_positions();

        // set the center as the center
        Position_set centers {{center_4.x, center_4.y},
                              {center_4.x +1, center_4.y},
                              {center_4.x, center_4.y + 1},
                              {center_4.x + 1, center_4.y + 1}};

        // check to see which parts are unoccupied, and pos to next moves if not
        Position_set valid_centers {};
        for (ge211::Posn<int> pos : centers) {
            if (Model::board_[pos] == Player::neither) {
                valid_centers[{pos}] = true;
                next_moves_[pos] = valid_centers;
                valid_centers.clear();
            }
        }
    } else {
        for (auto pos : Model::board()) {
            //check to make sure that this position is not occupied. if so,
            //skip this position
            if (Model::board_[pos] != Player::neither) {
                continue;
            }
            Position_set valids = evaluate_position_(pos);
            if (!valids.empty()) {
                next_moves_[pos] = valids;
            }
        }
    }
}

bool Model::advance_turn_()
{
    //OPTIONAL HELPER
    return false;
    // ^^^ this is wrong
}

void Model::set_game_over_()
{
    Model::turn_ = Player::neither;
    int black_count = 0;
    int white_count = 0;
    int empty = 0;
    // iterate over entire board and tally how many tiles belong to each team.
    for (auto pos : Model::board()){
        if (Model::board_[pos] == Player::dark){
            ++black_count;
        } else if (Model::board_[pos] == Player::light){
            ++white_count;
        } else {
            ++empty;
        }
    }
    // compare tile counts and determine winner.
    if (black_count == white_count){
        Model::winner_ = Player::neither;
    } else if (black_count > white_count){
        Model::winner_ = Player::dark;
    } else {
        Model::winner_ = Player::light;
    }
}

void Model::really_play_move_(Move move)
{
}
