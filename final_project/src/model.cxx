#include "model.hxx"

//for debug
#include <iostream>

using namespace ge211;

Model::Model()
        : board_({8,8}),
          next_moves_(),
          black_timer(true),
          white_timer (true)
{
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
    Move const* movep = find_move(start);
    if (!movep) {
        throw Client_logic_error("Model::play_move: no such move");
    } else {

        Position_set possible_moves = movep -> second;
        if (!possible_moves[end]) {
            throw Client_logic_error("Model::play_move: position to move"
                                     "to not found");
        }

        if (board_[end].get_piece_type() != Piece_type::null) {
            board_.remove_by_posn(end);
        }

        set_new_posn(start, end);
        castle_check(start, end);
        p_promo(end);

        if (turn() == Player::black) {
            pause_black();
            resume_white();
        } else if (turn() == Player::white) {
            pause_white();
            resume_black();
        }

        valid_moves_black = !no_moves_left(Player::black);
        valid_moves_white = !no_moves_left(Player::white);

        if (!valid_moves_black) {
            Model::turn_ = Player::neither;
            black_timer.pause();
            white_timer.pause();

            if (is_in_check(Player::black)) {
                winner_ = Player::white;
            } else {
                winner_ = Player::neither;
            }
            return;

        } else if (!valid_moves_white) {
            Model::turn_ = Player::neither;
            black_timer.pause();
            white_timer.pause();

            if (is_in_check(Player::white)) {
                winner_ = Player::black;
            } else {
                winner_ = Player::neither;
            }
            return;
        }

        if (black_timer.elapsed_time().seconds() > config_.time_limit) {
            winner_ = Player::white;
            turn_ = Player::out_of_time;
            return;
        } else if (white_timer.elapsed_time().seconds() > config_.time_limit) {
            winner_ = Player::black;
            turn_ = Player::out_of_time;
        }

        Model::turn_ = other_player(Model::turn_);
        Model::compute_next_moves_();

        //trying this out
        castle_check(start, end);
        Model::modify_next_moves_();

    }
}

// Helper for compute next moves. Given a certain position and a direction,
// will calculate all the possible moves for that piece in that position and
// in that direction. (Modified from reversi)
Position_set Model::moves_in_dir_(Position current, Dimensions dir) {

    Position_set moves_in_dir {};
    std::size_t n = 1;

    Position posn = current + n * dir;
    if (posn.x < 0 || posn.x >= Model::board_.dimensions().width
        || posn.y < 0 || posn.y >= Model::board_.dimensions().height) {
        return moves_in_dir;
    }

    while (board_[posn].get_piece_type() == Piece_type::null) {
        n++;

        posn = current + n * dir;
        if (posn.x < 0 || posn.x >= Model::board_.dimensions().width
            || posn.y < 0 || posn.y >= Model::board_.dimensions().height) {

            n--;
            break;
        }
    }

    if (board_[current + n * dir].get_player() == turn_) {n--;}

    for (std::size_t ii = 1; ii <= n; ii++) {
        moves_in_dir[{current + ii * dir}] = true;
    }

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
    Position_set possible_moves = Position_set();
    std::vector<Board::Dimensions> dirs_travel;

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

    size_t counter = 0;
    for (Dimensions dir : dirs_travel) {

        counter++;
        if (counter > 8)
        { break; }

        Position_set moves_in_dir = moves_in_dir_(p.get_posn(), dir);
        possible_moves.operator|=(moves_in_dir);
    }

    return possible_moves;
}

//calculate possible moves for pieces that can move a limited # of spaces
//includes pawn, knight, king
Position_set Model::spaces_ltd(Piece p)
{
    Position_set possible_moves = Position_set();
    std::vector<Board::Dimensions> dirs_travel;

    switch(p.get_piece_type()) {

    case Piece_type::pawn:

        if (p.get_player() == Player::black) {
            dirs_travel = board_.pawn_directions_dark();
        } else if (p.get_player() == Player::white) {
            dirs_travel = board_.pawn_directions_light();
        } else {
            throw Client_logic_error("Model::spaces_ltd: pawn cannot"
                                     "have player 'neither'");
        }

        dirs_travel = board_.modify_pawn_dirs(p, dirs_travel);
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

    size_t counter = 0;
    for (Dimensions dir : dirs_travel) {

        ++counter;
        if (counter > 8)
        { break; }

        Position posn = p.get_posn() + dir;
        if (posn.x < 0 || posn.x >= Model::board_.dimensions().width
            || posn.y < 0 || posn.y >= Model::board_.dimensions().height) {
            continue;

        } else if (board_[posn].get_piece_type() != Piece_type::null
                   && board_[posn].get_player() == p.get_player()) {
            continue;

        } else {
            possible_moves[posn] = true;
        }
    }
    return possible_moves;
}

// Computes next moves for every piece on the board for each player. (Modified
// from reversi)
void Model::compute_next_moves_()
{
    next_moves_.clear();

    for (auto pos : board_.all_positions()) {

        Piece piece = board_[pos];

        if (piece.get_player() == turn_) {
            switch(piece.get_piece_type()) {

                case Piece_type::rook: {
                    Position_set curr_moves = spaces_ult(piece);
                    next_moves_[pos] = curr_moves;
                    break;

                } case Piece_type::bishop: {
                    Position_set curr_moves = spaces_ult(piece);
                    next_moves_[pos] = curr_moves;
                    break;

                } case Piece_type::queen: {
                    Position_set curr_moves = spaces_ult(piece);
                    next_moves_[pos] = curr_moves;
                    break;

                } case Piece_type::pawn:{
                    Position_set curr_moves = spaces_ltd(piece);
                    next_moves_[pos] = curr_moves;
                    break;

                } case Piece_type::knight:  {
                    Position_set curr_moves = spaces_ltd(piece);
                    next_moves_[pos] = curr_moves;
                    break;

                } case Piece_type::king: {
                    Position_set curr_moves = spaces_ltd(piece);

                    if (Rrook_castle(piece.get_player())) {
                        if (piece.get_player() == Player::white) {
                            curr_moves[{6, 7}] = true;
                        } else {
                            curr_moves[{6, 0}] = true;
                        }
                    }

                    if (Lrook_castle(piece.get_player())) {
                        if (piece.get_player() == Player::white) {
                            curr_moves[{2, 7}] = true;
                        } else {
                            curr_moves[{2, 0}] = true;
                        }
                    }
                    next_moves_[pos] = curr_moves;
                    break;

                } default: {
                    return;
                }
            }
        }
    }
}

//takes in next_moves_ and removes any moves from the position
void Model::modify_next_moves_()
{
    Model m = *this;
    Player p = m.turn_;

    for (Move& move : next_moves_) {
        Position start = move.first;
        for (Position end : move.second) {

            m.set_new_posn(start,end);
            m.compute_next_moves_();

            if (m.is_in_check(p)) {
                move.second[end] = false;
            }

            m.set_new_posn(end, start);
        }
    }
}

//takes in a position where the piece starts, a piece where it ends, and
//changes the position of that piece
void Model::set_new_posn(Position start, Position end) {

    Piece p = board_[start];

    if (p.get_piece_type() == Piece_type::null) {
        throw Client_logic_error("Model::set_new_posn: no piece at "
                                 "this start posn");
    }

    board_.change_piece_posn(p, end);
}

// Calculates the if the given player is in check. Ultimately a helper
// function for checkmate.
bool Model::is_in_check(Player p) const{

    Model m = *this;
    m.turn_ = other_player(p);
    Position king_posn = m.board_.find_king_location(p);
    m.compute_next_moves_();

    for (Move move : m.next_moves_) {

        for (Position posn : move.second) {
            if (posn == king_posn) {
                return true;
            }
        }
    }

    return false;
}

// Helper for checkmate. Calculates if the given player has no moves left,
// which can lead to a stalemate or a checkmate depending on a variety of
// factors on the board.
bool Model::no_moves_left(Player p) const
{
   Model m = *this;
   m.turn_ = p;
   m.compute_next_moves_();
   m.modify_next_moves_();

   for (Move move : m.next_moves_) {

       if (!move.second.empty()) {
           return false;
       }
   }

    return true;
}

// Helper function for compute/play moves. Takes in a variety of positions on
// the board to see whether or not castling is a valid move option for the
// king. Also detects if the player has forfeited castling by moving the
// king/rooks.
void Model::castle_check(Position start, Position end) {

    Position king_xy = board_.find_king_location(turn_);

    if ((turn_ == Player::white && king_xy != Position{4,7})
            || (turn_ == Player::black && king_xy != Position{4,0})){

        if(BRcast && !Bcastle && king_xy == Position{6,0}){
            set_new_posn({7, 0}, {5, 0});
            Bcastle = true;

        } else if(BLcast && !Bcastle && king_xy == Position{2,0}){
            set_new_posn({0, 0}, {3, 0});
            Bcastle = true;

        }else if(WRcast && !Wcastle && king_xy == Position{6,7}){
            set_new_posn({7, 7}, {5, 7});
            Wcastle = true;

        }else if(WLcast && !Wcastle && king_xy == Position{2,7}){
            set_new_posn({0, 7}, {3, 7});
            Wcastle = true;

        }else if(board_[end].get_player() == Player::white){
            Wcastle = true;
        } else if (board_[end].get_player() == Player::black){
            Bcastle = true;
        }
    }


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
                return true;
            } else {
                return false;
            }
        } case Player::black: {
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
        } default: {
            return false;
        }
    }
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
        } case Player::black: {
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
        } default: {
            return false;
        }
    }
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

// Returns piece type at a given position.
Piece_type Model::return_piece_type(Position posn)
{
    Piece piece = board_[posn];
    return piece.get_piece_type();
}


