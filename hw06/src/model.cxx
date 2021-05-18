 #include "model.hxx"


using namespace ge211;

Model::Model(int size)
        : Model(size, size)
{ }

Model::Model(int width, int height)
        : board_({width, height}),
        next_moves_()
{
    // TODO: initialize `next_moves_` to `turn_`'s available moves

    //start off with the first four center positions filled in
    //contains members {posn, {width,height}}
    Rectangle center_4 = Model::board_.center_positions();

    //start at the position given by the rectangle, then find the coords
    //in terms of board position {2,3}, {4,1} etc. of corner squares
    Position posn_tl{center_4.x, center_4.y};
    Position posn_tr{center_4.x +1, center_4.y};
    Position posn_bl{center_4.x, center_4.y + 1};
    Position posn_br{center_4.x + 1, center_4.y + 1};

    //fill in the center 4 tiles with alternating colors. advance the
    //turn as you go
    Position_set black_moves{posn_tl,posn_br};
    Position_set white_moves{posn_tr,posn_bl};

    //Model::board_.set_all(Position_set{posn_tl, posn_br}, Model::turn_);
    Model::board_.set_all(black_moves, Model::turn_);
    Model::turn_ = other_player(Model::turn_);

    //Model::board_.set_all(Position_set{posn_bl, posn_tr}, Model::turn_);
    Model::board_.set_all(white_moves, Model::turn_);
    Model::turn_ = other_player(Model::turn_);


    //make a Move_map called next_moves_ that contains all the possible
    //next moves for the board. For now, let's make all moves possible
    //so we can click anywhere on the board
    //first: posn<int>, the tile you place
    //second: Position_set, the sum of all tiles you gain by playing "first"

    //Move_map Model::next_moves_ = {};

    //iterate through all of the positions in board


    for (Position posn : Model::board()) {
        Model::next_moves_[posn] = {posn};
    }

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

    //advance the turn
    if (Model::turn() == Player::dark) {
        Model::turn_ = Player::light;
    } else if (Model::turn() == Player::light) {
        Model::turn_ = Player::dark;
    }

}

//
// BELOW ARE *OPTIONAL* HELPER FUNCTIONS
//

Position_set Model::find_flips_(Position current, Dimensions dir) const
{
    Position_set flips {};
    // yeah I know i did this with a while when i probably could have done a
    // for, but i can worry about that later
    std::size_t n = 1;
    while (board_[current + n * dir] == other_player(turn_)){
        n++; // increments n when we detect the tile is occupied by a
    };

    if (board_[current + n * dir] == turn_)
    {
        // if the final tile is the current player, then we add the
        // positions up to that one to the position set.
        for(std::size_t ii = 1; ii < n; ii++){
            flips[{current + ii * dir}] = true;
        }
    }
    return flips;
}

Position_set Model::evaluate_position_(Position pos) const
{
    Position_set curr_pos {};
    // iterate through all possible directions and call find_flips
    for (auto dim : Board::all_directions())
    {
        Position_set new_tiles = find_flips_(pos, dim);
        curr_pos.operator|=(new_tiles); // update the curr_pos with flips
    };
    return curr_pos;
}

void Model::compute_next_moves_()
{
    // TODO OR NOT TODO: OPTIONAL HELPER
    // iterate through entire board all call eval_position on every spot.
    // Only add non empty position sets  to next_moves_.
    next_moves_.clear(); // first clear out next moves
    for (auto pos : Model::board()){
        auto valids = evaluate_position_(pos);
        if (!valids.empty()){
            next_moves_[pos] = valids;
        }
    }
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
