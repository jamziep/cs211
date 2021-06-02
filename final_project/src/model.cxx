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

bool BRcast = false;
bool BLcast = false;
bool WRcast = false;
bool WLcast = false;
bool Wcastle = false;
bool Bcastle = false;

void Model::play_move(Position start, Position end)
{
    // if (is_game_over())
    //     //rather than throw an error, run our own game over function
    //     set_game_over_();

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

        // Castle check. This has been done fairly poorly but we are running
        // out of time so it's not the biggest concern.

        if (board_[start].get_piece_type() == Piece_type::king)
        {
            if(BRcast && !Bcastle){
                if (end.x == 6 && end.y ==0) {
                    set_new_posn({7, 0}, {5, 0});
                }
                Bcastle = true;
            } else if(BLcast && !Bcastle){
                if (end.x == 2 && end.y == 0) {
                    set_new_posn({0, 0}, {3, 0});
                }
                Bcastle = true;
            }else if(WRcast && !Wcastle){
                if (end.x == 6 && end.y == 7) {
                    set_new_posn({7, 7}, {5, 7});
                }
                Wcastle = true;
            }else if(WLcast && !Wcastle){
                if (end.x == 2 && end.y == 7) {
                    set_new_posn({0, 7}, {3, 7});
                }
                Wcastle = true;
            }
        }

        //set the new position of the piece
        set_new_posn(start, end);

        // Pawn promotion check. Auto-promotes to queen if the pawn is moved
        // to an end position in the opponent's back rank.
        p_promo(end);


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


        //check to see if we're at an end state (Commented out for debug)

        // if (is_checkmate(Player::black)) {
        //     winner_ = Player::white;
        //     set_game_over_();
        // } else if (is_checkmate(Player::white)) {
        //     winner_ = Player::black;
        //     set_game_over_();
        // }



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

    //if last tile in this direction is same as current player, can't move there
    if (board_[current + n * dir].get_player() == turn_) {n--;}

    for(std::size_t ii = 1; ii <= n; ii++){
        moves_in_dir[{current + ii * dir}] = true;
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
        // COMMENTED OUT FOR NOW
        // board_.modify_pawn_dirs(p, dirs_travel);
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
    if(p.get_piece_type() != Piece_type::pawn) {
        for (Dimensions dir : dirs_travel) {

            //find the first position in the direction where we're looking
            //if it's out of bounds, skip
            Position posn = p.get_posn() + dir;
            if (posn.x < 0 || posn.x >= Model::board_.dimensions().width
                || posn.y < 0 || posn.y >= Model::board_.dimensions().height) {
                continue;

                //if the square in that direction is occupied by a piece of the
                //same color as this piece, skip
            } else if (board_[posn].get_piece_type() != Piece_type::null
                       && board_[posn].get_player() == p.get_player()) {
                continue;

                //else: the position is either free or occupied by the opposite
                //player
            } else {
                possible_moves[posn] = true;
            }
        }
    } else {
        // custom pawns since the helper in board didnt work quite right
        // will do the check for diagonal takes and moving past the first rank
        Player what_player = p.get_player();
        Position what_posn = p.get_posn();
        for (Dimensions dir : dirs_travel) {
            // normal bound checks and what not.
            Position posn = p.get_posn() + dir;
            if (posn.x < 0 || posn.x >= Model::board_.dimensions().width
                || posn.y < 0 || posn.y >= Model::board_.dimensions().height) {
                continue;
            } else if (board_[posn].get_piece_type() != Piece_type::null
                       && board_[posn].get_player() == p.get_player()) {
                continue;
            } else {
                // first check if pawn is not blocked and in first rank
                if (what_player==Player::white && what_posn.y!=6 && posn
                .y+2==what_posn.y) {
                        possible_moves[posn] = false;

                } else if (what_player==Player::black && what_posn.y!=1 &&
                posn.y-2 == what_posn.y) {
                        possible_moves[posn] = false;

                    // check to see if posn is diagonal and detects a piece
                } else if (posn.x-1 == what_posn.x || posn.x+1 == what_posn.x){
                    if (operator[](posn).get_piece_type() != Piece_type::null) {
                        possible_moves[posn] = true;
                    }
                    // check to see if the posn in front of piece is blocked.
                }else if(operator[]({what_posn.x,what_posn.y-1})
                .get_piece_type() == Piece_type::null && what_player
                ==Player::white){
                    possible_moves[posn] = true;

                }else if(operator[]({what_posn.x,what_posn.y+1})
                .get_piece_type() == Piece_type::null && what_player ==
                Player::black){
                    possible_moves[posn] = true;
                }
            }
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

        if (piece.get_piece_type() != Piece_type::null
                && piece.get_player() == turn_) {

            //get the possible positions of motion for this turn; add Move
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
                // check for castling. Adds the move to the moves of the king.
                if (piece.get_piece_type() == Piece_type::king) {
                    if (bool castling = Rrook_castle(piece.get_player())) {
                        if (piece.get_player() == Player::white) {
                            curr_moves[{6, 7}] = true;
                        } else {
                            curr_moves[{6, 0}] = true;
                        }
                    }
                    if (bool castling = Lrook_castle(piece.get_player())) {
                        if (piece.get_player() == Player::white) {
                            curr_moves[{2, 7}] = true;
                        } else {
                            curr_moves[{2, 0}] = true;
                        }
                    }
                }

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

// //checks if a a model "m" (NOT the current model used in model.is_in_check()
// //uses) has a player in check
// bool Model::is_in_check(Player p) const{
//
//     //ensure next_moves_ is calculated for the opposite player as p
//     Player temp = turn();
//     turn_ = other_player(p);
//
//     //find the location of the king in this player's piece_set
//     // Position king_posn = board_.find_king_location(p);
//     Position king_posn = board_.find_king_location(p);
//
//     //compute next moves for the other player of the piece we're checking
//     compute_next_moves_();
//
//     //if the opposite player has a move that could threaten the
//     //king, the current player is in check
//     for (Move move : next_moves_) {
//
//         //iterate through all the places where this piece can move and
//         //see if player p's king is at one of those pieces
//         for (Position posn : move.second) {
//             if (posn == king_posn) {
//
//                 //reset the turn of this model and revert next_moves_
//                 //to what it was before
//                 turn_ = temp;
//                 compute_next_moves_();
//                 return true;
//             }
//         }
//     }
//
//     //ensure the turn gets set back to what it was before this function
//     turn_ = temp;
//     compute_next_moves_();
//
//     //if none of the moves of the other player
//     //could put the king in danger, curr player is not in check
//     return false;
// }

bool Model::is_in_check(Player p) const{

    //make a copy of the board so this can be const
    Model m = *this;
    m.turn_ = other_player(p);
    Position king_posn = m.board_.find_king_location(p);
    m.compute_next_moves_();

    //if the opposite player has a move that could threaten the
    //king, the current player is in check
    for (Move move : m.next_moves_) {

        //iterate through all the places where this piece can move and
        //see if player p's king is at one of those pieces
        for (Position posn : move.second) {
            if (posn == king_posn) {

                //reset the turn of this model and revert next_moves_
                //to what it was before
                // turn_ = temp;
                // compute_next_moves_();
                return true;
            }
        }
    }

    //ensure the turn gets set back to what it was before this function
    // turn_ = temp;
    // compute_next_moves_();

    //if none of the moves of the other player
    //could put the king in danger, curr player is not in check
    return false;
}


bool Model::is_checkmate(Player p) const
{
    // //first, check if the current player is in check
    // if (!is_in_check(p)) {
    //     return false;
    // }
    //
    // //set the current player to piece p. revert after function is over
    // Player temp = turn_;
    // turn_ = p;
    // compute_next_moves_();
    //
    // //make a copy of the model and analyze all possible moves
    // Model other = *this;
    //
    // //player is in check. let's see if there are any moves the
    // //player could make that get the king out of check
    // for (Move move : next_moves_) {
    //
    //     //see if moving this piece to any of its possible positions
    //     //gets the king out of check
    //     Position start = move.first;
    //     for (Position end : move.second) {
    //         other.turn_ = p;
    //         other.play_move(start, end);
    //         if (!other.is_in_check(p)) {
    //
    //             //if player p is not in check after a move, then success--
    //             //player is not in checkmate
    //             return false;
    //         }
    //     }
    //
    //
    // }
    //
    // //revert the changes we made to the turn and moves
    // turn_ = temp;
    // compute_next_moves_();
    //
    // //if we've gotten here, means the player IS in checkmate--
    // //none of the possible moves can get the player out of checkmate
    return true;
}
// two helpers for determining if castling is a valid move. Separated into
// right and left rooks. Adds moves to next_moves within
// compute_next_moves if the piece is a king.
bool Model::Rrook_castle (Player plr)
{
    switch(plr) {
    case Player::white: {
        Piece WKing = board_[{4, 7}];
        Piece Wrook = board_[{7, 7}];
        if (WKing.get_piece_type() == Piece_type::king &&
            Wrook.get_piece_type() == Piece_type::rook &&
            board_[{6, 7}].get_piece_type() == Piece_type::null &&
            board_[{5, 7}].get_piece_type() == Piece_type::null &&
            !Wcastle) {
            WRcast = true;
            std::cout << "we got here";
            return true;
        } else {
            return false;
        }
    }
    case Player::black: {
        Piece BKing = board_[{4, 0}];
        Piece Brook = board_[{7, 0}];
        if (BKing.get_piece_type() == Piece_type::king &&
            Brook.get_piece_type() == Piece_type::rook &&
            board_[{6, 0}].get_piece_type() == Piece_type::null &&
            board_[{5, 0}].get_piece_type() == Piece_type::null &&
            !Bcastle) {
            BRcast = true;
            return true;
        } else {
            return false;
        }
    }
    case Player::neither:
    {}
    }
    return false; // default case
}

bool Model::Lrook_castle (Player plr)
{
    switch(plr) {
    case Player::white: {
        Piece WKing = board_[{4, 7}];
        Piece Wrook = board_[{0, 7}];
        if (WKing.get_piece_type() == Piece_type::king &&
            Wrook.get_piece_type() == Piece_type::rook &&
            board_[{1, 7}].get_piece_type() == Piece_type::null &&
            board_[{2, 7}].get_piece_type() == Piece_type::null &&
            board_[{3, 7}].get_piece_type() == Piece_type::null &&
            !Wcastle) {
            WLcast = true;
            return true;
        } else {
            return false;
        }
    }
    case Player::black: {
        Piece BKing = board_[{4, 0}];
        Piece Brook = board_[{0, 0}];
        if (BKing.get_piece_type() == Piece_type::king &&
            Brook.get_piece_type() == Piece_type::rook &&
            board_[{1, 0}].get_piece_type() == Piece_type::null &&
            board_[{2, 0}].get_piece_type() == Piece_type::null &&
            board_[{3, 0}].get_piece_type() == Piece_type::null &&
            !Bcastle) {
            BLcast = true;
            return true;
        } else {
            return false;
        }
    }
    case Player::neither:
    {}
    }
    return false; // default case
}

// Pawn promotion. If the given position is identified to be in the back rank
// of the opponent, then we promote the piece.
void Model::p_promo(Position pos)
{
    Piece p = board_[pos];
    Player plr = p.get_player();
    Piece_type type = p.get_piece_type();
    if (p.get_piece_type() == Piece_type::null) {
        throw Client_logic_error("Model::p_promo: no piece at "
                                 "This posn");
    }

    if (plr==Player::white && type==Piece_type::pawn && pos.y==0){
        board_.set_piece_as(p);
    } else if(plr==Player::black && type==Piece_type::pawn && pos.y==7){
        board_.set_piece_as(p);
    }

}




void Model::set_game_over_()
{
    Model::turn_ = Player::neither;
}


