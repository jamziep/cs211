#include "model.hxx"

//for debug
#include <iostream>

using namespace ge211;


Model::Model()
        : board_({8,8}),
          next_moves_(),
          black_timer(true),
          white_timer (true)
          //pieces_taken_()
{

    //initialize next_moves_ to turn_'s possible next moves,
    //using the compute_next_moves helper
    Model::compute_next_moves_();
}


Model::Rectangle Model::board() const
{
    return board_.all_positions();
}

Piece Model::operator[](Position pos)
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

void Model::play_move(Position start, Position end)
{
    if (is_game_over())
        //rather than throw an error, run our own game over function
        set_game_over_();

    //see if there is a piece at the position where player wants to play
    Move const* movep = find_move(start);
    if (!movep) {
        //now that we've implemented checks for this in controller, we actually
        //do want this error to throw if we get here, b/c it means we did some-
        //thing wrong
        throw Client_logic_error("Model::play_move: no such move");
    } else {

        //check to see that the position the player wants to move their
        //piece to exists. i.e. check that "end" is in movep -> second
        Position_set possible_moves = movep -> second;
        if (!possible_moves[end]) {
            throw Client_logic_error("Model::play_move: position to move"
                                     "to not found");
        }

        //if there was already a piece at the place where the
        //move was going to be made, remove piece from board
        if (board_[end].get_piece_type() != Piece_type::null) {
            board_.remove_by_posn(end);
        }

        //set the new position of the piece
        set_new_posn(start, end);

        //pause the timer for the current player, start the timer
        //for the other timer
        if (turn() == Player::black) {
            pause_black();
            resume_white();
        } else if (turn() == Player::white) {
            pause_white();
            resume_black();
        } else {
            //case where current turn is player "neither"
        }

        //advance the turn. using function from player.cxx
        Model::turn_ = other_player(Model::turn_);

        //refill next_moves_ for the current player
        Model::compute_next_moves_();
    }
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

//in each direction of travel, take the current posn of piece
//and add the vector of the dir_travel to it.
// - bounds-check the posn to make sure it's between (0,0) and (7,7)
//      --this happens in [], but could throw an error
// - move as many free spaces as possible in that direction
// - if there's a piece at the end of the line, check if it's an enemy
// - piece, in which case we'd be able to take that piece
Position_set Model::spaces_ult(Piece p)
{
    //initialize sets of data of unknown size
    Position_set possible_moves = Position_set();
    std::vector<Board::Dimensions> dirs_travel;

    //find the directions of travel possible for this piece
    switch(p.get_piece_type()) {

    case Piece_type::rook:
        dirs_travel = board_.rook_directions();
        break;
    case Piece_type::bishop:
        dirs_travel = board_.bishop_directions();
        break;
    case Piece_type::queen:
        dirs_travel = board_.all_directions();
        break;
    default:
        throw Client_logic_error("Model::spaces_ult: piece must be r/b/q");
    }

    for (Dimensions dir : dirs_travel) {

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
    //initialize sets of data of unknown size
    Position_set possible_moves = Position_set();
    std::vector<Board::Dimensions> dirs_travel;

    //find the directions of travel possible for this piece
    switch(p.get_piece_type()) {

    case Piece_type::pawn:

        //pawn has different possible directions of travel depending
        //on which side of the board they're on
        if (p.get_player() == Player::black) {
            dirs_travel = board_.pawn_directions_dark();
        } else if (p.get_player() == Player::white) {
            dirs_travel = board_.pawn_directions_light();
        } else {
            throw Client_logic_error("Model::spaces_ltd: pawn cannot"
                                     "have player 'neither'");
        }
        //pawn has special directions of travel depending on what's around
        //it, so account for there. do using a reference to dirs_travel
        board_.modify_pawn_dirs(p, dirs_travel);
        break;
    case Piece_type::knight:
        dirs_travel = board_.knight_directions();
        break;
    case Piece_type::king:
        dirs_travel = board_.all_directions();
        break;
    default:
        throw Client_logic_error("Model::spaces_ult: piece must be kn/ki/p");
    }

    //in each direction of travel, take the current posn of piece
    //and add the vector of the dir_travel to it.
    // - bounds-check the posn to make sure it's between (0,0) and (7,7)
    //      --this happens in [], but could throw an error
    // - move as many free spaces as possible in that direction
    // - if there's a piece at the end of the line, check if it's an enemy
    // - piece, in which case we'd be able to take that piece
    for (Dimensions dir : dirs_travel ) {

        //find the first position in the direction where we're looking
        //if it's out of bounds, skip
        Position posn = p.get_posn() + dir;
        if (posn.x < 0 || posn.x >= Model::board_.dimensions().width
            || posn.y < 0 || posn.y >= Model::board_.dimensions().height) {
            continue;

        //if the square in that direction is occupied by a piece of the
        //same color as this piece, skip
        } else if (board_[posn].get_piece_type() != Piece_type::null
            && board_[posn].get_player() == p.get_player() ){
            continue;

        //else: the position is either free or occupied by the opposite
        //player
        } else {
            possible_moves[posn] = true;
        }
    }

    //return all unoccupied or enemy-occupied spaces
    return possible_moves;
}


void Model::compute_next_moves_()
{
    // iterate through entire board all call eval_position on every spot.
    // Only add non empty position sets  to next_moves_.
    next_moves_.clear(); // first clear out next moves

    //iterate through each space on the board and find the moves associated
    //with that piece
    for (auto pos : board_.all_positions()) {

        //get the piece at this position, if any
        Piece piece = board_[pos];

        //conditions: the piece must exist and the piece must be of
        //the current player, given by move)

        //other condition that hasn't been checked for: that the piece
        //at this position is active. piece_set may take care of this
        if (piece.get_piece_type() != Piece_type::null
                && piece.get_player() == turn_) {

            //get the possible positions of motion for this turn;
            //add a Move containing {position of curr piece, all possible
            //places to move} to next_moves_
            if (piece.get_piece_type() == Piece_type::bishop
                    || piece.get_piece_type() == Piece_type::rook
                    || piece.get_piece_type() == Piece_type:: queen) {

                //for the pieces that have unlimited movement
                Position_set curr_moves = spaces_ult(piece);
                next_moves_[pos] = curr_moves;

            } else if (piece.get_piece_type() == Piece_type::pawn
                    || piece.get_piece_type() == Piece_type::knight
                    || piece.get_piece_type() == Piece_type::king) {

                //for the pieces that have limited movement
                Position_set curr_moves = spaces_ltd(piece);
                next_moves_[pos] = curr_moves;
            }
        }
    }
}

//takes in a position where the piece starts, a piece where it ends, and
//changes the position of that piece
void Model::set_new_posn(Position start, Position end) {

    //get the piece at a given posn from board
    Piece p = board_[start];

    //if no piece is found, throw error
    if (p.get_piece_type() == Piece_type::null) {
        throw Client_logic_error("Model::set_new_posn: no piece at "
                                 "this start posn");
    }

    //modify the position of this piece
    board_.change_piece_posn(p, end);
}

//checks if a a model "m" (NOT the current model used in model.is_in_check()
//uses) has a player in check
bool Model::is_in_check(Player p){

    //ensure next_moves_ is calculated for the opposite player as p
    Player temp = turn();
    turn_ = other_player(p);

    //find the location of the king in this player's piece_set
    // Position king_posn = board_.find_king_location(p);
    Position king_posn = board_.find_king_location(p);

    //compute next moves for the other player of the piece we're checking
    compute_next_moves_();

    //if the opposite player has a move that could threaten the
    //king, the current player is in check
    for (Move move : next_moves_) {

        //iterate through all the places where this piece can move and
        //see if player p's king is at one of those pieces
        for (Position posn : move.second) {
            if (posn == king_posn) {

                //reset the turn of this model and revert next_moves_
                //to what it was before
                turn_ = temp;
                compute_next_moves_();
                return true;
            }
        }
    }

    //ensure the turn gets set back to what it was before this function
    turn_ = temp;
    compute_next_moves_();

    //if none of the moves of the other player
    //could put the king in danger, curr player is not in check
    return false;
}

bool Model::is_checkmate(Player p)
{
    //first, check if the current player is in check
    if (!is_in_check(p)) {
        return false;
    }

    //set the current player to piece p. revert after function is over
    Player temp = turn_;
    turn_ = p;
    compute_next_moves_();

    //make a copy of the model and analyze all possible moves
    Model other = *this;

    //player is in check. let's see if there are any moves the
    //player could make that get the king out of check
    for (Move move : next_moves_) {

        //see if moving this piece to any of its possible positions
        //gets the king out of check
        Position start = move.first;
        for (Position end : move.second) {
            other.turn_ = p;
            other.play_move(start, end);
            if (!other.is_in_check(p)) {

                //if player p is not in check after a move, then success--
                //player is not in checkmate
                return false;
            }
        }


    }

    //revert the changes we made to the turn and moves
    turn_ = temp;
    compute_next_moves_();

    //if we've gotten here, means the player IS in checkmate--
    //none of the possible moves can get the player out of checkmate
    return true;
}

void Model::set_game_over_()
{
    Model::turn_ = Player::neither;
}


