 #include "model.hxx"


using namespace ge211;

Model::Model(int size)
        : Model(size, size)
{ }

Model::Model(int width, int height)
        : board_({width, height})
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
    // Position posn_tl{1,1};
    // Position posn_tr{1,6};
    // Position posn_bl{5,2};
    // Position posn_br{4,5};

    //fill in the center 4 tiles with alternating colors. advance the
    //turn as you go

    //expected behavior: two black tiles at top left and bottom right,
    //two white tiles at top right and bottom left

    //actual behavior:
    // - no tile exists in bottom left  position
    // - changing the x position of posn_bl moves the white tile first
    //   in the x direction, but it also affects what other tiles show up
    //      - ex. changing posn_bl.x affects the top right tile too

    Position_set black_moves{posn_tl,posn_br};
    Position_set white_moves{posn_tr,posn_bl};


    //Model::board_.set_all(Position_set{posn_tl, posn_br}, Model::turn_);
    Model::board_.set_all(black_moves, Model::turn_);
    Model::turn_ = other_player(Model::turn_);

    //Model::board_.set_all(Position_set{posn_bl, posn_tr}, Model::turn_);
    Model::board_.set_all(white_moves, Model::turn_);
    Model::turn_ = other_player(Model::turn_);

    // //let's try doing this one at a time
    // Model::board_.set_all(Position_set{posn_tl},Model::turn_);
    // Model::turn_ = other_player(Model::turn_);
    //
    // Model::board_.set_all(Position_set{posn_tr},Model::turn_);
    // Model::turn_ = other_player(Model::turn_);
    //
    // Model::board_.set_all(Position_set{posn_bl},Model::turn_);
    // Model::turn_ = other_player(Model::turn_);
    //
    // Model::board_.set_all(Position_set{posn_br},Model::turn_);
    // Model::turn_ = other_player(Model::turn_);

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
