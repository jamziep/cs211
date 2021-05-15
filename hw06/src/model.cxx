 #include "model.hxx"

using namespace ge211;

Model::Model(int size)
        : Model(size, size)
{ }

Model::Model(int width, int height)
        : board_({width, height})
{
    // TODO: initialize `next_moves_` to `turn_`'s available moves
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
        throw Client_logic_error("Model::play_move: game over");

    Move const* movep = find_move(pos);
    if (!movep)
        throw Client_logic_error("Model::play_move: no such move");

    // TODO: actually execute the move, advance the turn, refill
    // next_moves_, etc.

    //FOR NOW: just so we can visualize what's being created in the board,
    //this function will assume all moves are valid. Change this assumption
    //later.

    //find the position set of all things we've changed via find_move()
    Position_set pset = movep -> second;
    Player turn = Model::turn();

    //add the position set of all the things we've changed via find_move()
    //to the data of which tiles are where in board
    Model::board_.set_all(pset, turn);
}

//
// BELOW ARE *OPTIONAL* HELPER FUNCTIONS
//

Position_set Model::find_flips_(Position current, Dimensions dir) const
{
    // TODO OR NOT TODO: OPTIONAL HELPER
    Position_set flips {};
    // find position, draw line all directions until you hit the same color
    // all positions that are in between that position and your current are flips
    // Use board::all_directions?
    for (auto dim : Board::all_directions()){

    };

    return flips;
    // ^^^ this is wrong
}

Position_set Model::evaluate_position_(Position pos) const
{
    // TODO OR NOT TODO: OPTIONAL HELPER
    return {};
    // ^^^ this is wrong
}

void Model::compute_next_moves_()
{
    // TODO OR NOT TODO: OPTIONAL HELPER
}

bool Model::advance_turn_()
{
    // TODO OR NOT TODO: OPTIONAL HELPER
    return false;
    // ^^^ this is wrong
}

void Model::set_game_over_()
{
    // TODO OR NOT TODO: OPTIONAL HELPER
}

void Model::really_play_move_(Move move)
{
    // TODO OR NOT TODO: OPTIONAL HELPER
}
