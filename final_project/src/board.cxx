#include "board.hxx"

#include <algorithm>

using namespace ge211;

//the Piece_sets that represent the initial state of
//black player and white

std::vector<Piece> black_start_moves{
        Piece(Piece_type::pawn, Player::black,{0,1} ),
        Piece(Piece_type::pawn, Player::black,{1,1} ),
        Piece(Piece_type::pawn, Player::black,{2,1} ),
        Piece(Piece_type::pawn, Player::black,{3,1} ),
        Piece(Piece_type::pawn, Player::black,{4,1} ),
        Piece(Piece_type::pawn, Player::black,{5,1} ),
        Piece(Piece_type::pawn, Player::black,{6,1} ),
        Piece(Piece_type::pawn, Player::black,{7,1} ),

        Piece(Piece_type::rook, Player::black,{0,0} ),
        Piece(Piece_type::knight, Player::black,{1,0} ),
        Piece(Piece_type::bishop, Player::black,{2,0} ),
        Piece(Piece_type::queen, Player::black,{3,0} ),
        Piece(Piece_type::king, Player::black,{4,0} ),
        Piece(Piece_type::bishop, Player::black,{5,0} ),
        Piece(Piece_type::knight, Player::black,{6,0} ),
        Piece(Piece_type::rook, Player::black,{7,0} ),
};

std::vector<Piece> white_start_moves{
        Piece(Piece_type::pawn, Player::white,{0,6} ),
        Piece(Piece_type::pawn, Player::white,{1,6} ),
        Piece(Piece_type::pawn, Player::white,{2,6} ),
        Piece(Piece_type::pawn, Player::white,{3,6} ),
        Piece(Piece_type::pawn, Player::white,{4,6} ),
        Piece(Piece_type::pawn, Player::white,{5,6} ),
        Piece(Piece_type::pawn, Player::white,{6,6} ),
        Piece(Piece_type::pawn, Player::white,{7,6} ),


        Piece(Piece_type::rook, Player::white,{0,7} ),
        Piece(Piece_type::knight, Player::white,{1,7} ),
        Piece(Piece_type::bishop, Player::white,{2,7} ),
        Piece(Piece_type::queen, Player::white,{3,7} ),
        Piece(Piece_type::king, Player::white,{4,7} ),
        Piece(Piece_type::bishop, Player::white,{5,7} ),
        Piece(Piece_type::knight, Player::white,{6,7} ),
        Piece(Piece_type::rook, Player::white,{7,7} ),
};


Board::Board(Dimensions dims)
        : dims_(dims),
        white_(white_start_moves),
        black_(black_start_moves)
{
    if (dims_.width < 2 || dims_.height < 2) {
        throw Client_logic_error("Board::Board: dims too small");
    }

    if (dims_.width > Position_set::coord_limit ||
        dims_.height > Position_set::coord_limit) {
        throw Client_logic_error("Board::Board: dims too large");
    }
}

Board::Dimensions
Board::dimensions() const
{
    return dims_;
}

bool
Board::good_position(Position pos) const
{
    return 0 <= pos.x && pos.x < dims_.width &&
           0 <= pos.y && pos.y < dims_.height;
}

Piece
Board::operator[](Position pos) const
{
    bounds_check_(pos);
    return get_piece_(pos);
}


//take in a piece (not a reference), go into the board,
//and modify the position of a piece at that same position
//using board.white_ or black_
void Board::change_piece_posn(Piece p, Position end) {

    Position start = p.get_posn();

    if (p.get_player() == Player::white) {
        white_.change_posn(start, end);
    } else if (p.get_player() == Player::black) {
        black_.change_posn(start, end);
    } else {
        throw Client_logic_error("Model::change_piece_posn: cannot"
                                 "change the position of Player::neither");
    }
}

//Find the king in either of the piece_sets, and return its location.
//Can apply to either player and either piece set.

Position Board::find_king_location(Player p) const
{
    if (p == Player::black) {
        Piece_set pcset = black_;

        for (size_t ii = 0; ii < pcset.size(); ++ii) {
            Piece curr_piece = pcset[ii];

            if (curr_piece.get_piece_type() == Piece_type::king) {
                return curr_piece.get_posn();
            }
        }

        //if no king is found
        throw Client_logic_error("Board::find_king_location: no king"
                                 " found for dark");

    } else if (p == Player::white) {
        Piece_set pcset = white_;

        for (size_t ii = 0; ii < pcset.size(); ++ii) {
            Piece curr_piece = pcset[ii];
            if (curr_piece.get_piece_type() == Piece_type::king) {
                return curr_piece.get_posn();
            }
        }

        throw Client_logic_error("Board::find_king_location: no king"
                                 " found for light");

    } else {
        throw Client_logic_error("Board::find_king_location: can't find"
                                 " a king for a non black/white player");
    }
}

//First, finds a piece that matches this posn, then goes into the
//piece_set and "removes" the piece. Really does this by setting
//piece.active_ = false, so it can't be played anymore.

void Board::remove_by_posn(Position posn) {

    Piece piece = operator[](posn);

    if (piece.get_player() == Player::white) {
        white_.remove(piece);
    } else if (piece.get_player() == Player::black) {
        black_.remove(piece);
    } else {
        throw Client_logic_error("Board::remove_by_posn: can't remove"
                                 " a player of type 'neither'");
    }
}

static std::vector<Board::Dimensions>
build_directions()
{
    std::vector<Board::Dimensions> result;

    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx || dy) {
                result.push_back({dx, dy});
            }
        }
    }

    return result;
}

std::vector<Board::Dimensions> const&
Board::all_directions()
{
    static std::vector<Dimensions> result = build_directions();
    return result;
}

//extra directions for the travel of each piece
std::vector<Board::Dimensions> const&
Board::knight_directions()
{
    static std::vector<Board::Dimensions> result;

    result.push_back({1,2});
    result.push_back({1,-2});
    result.push_back({-1,2});
    result.push_back({-1,-2});

    result.push_back({2,1});
    result.push_back({2,-1});
    result.push_back({-2,1});
    result.push_back({-2,-1});

    return result;
}

std::vector<Board::Dimensions> const&
Board::rook_directions()
{
    //modify this so it just has horizontal directions
    //use an XOR: +/-x or +/-y, but not both
    static std::vector<Board::Dimensions> result;

    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if ((dx || dy) && !(dx && dy)) {
                result.push_back({dx, dy});
            }
        }
    }

    return result;
}

std::vector<Board::Dimensions> const&
Board::bishop_directions()
{
    static std::vector<Board::Dimensions> result;

    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx && dy) {
                result.push_back({dx, dy});
            }
        }
    }

    return result;
}

std::vector<Board::Dimensions> const&
Board::pawn_directions_light()
{
    static std::vector<Board::Dimensions> result;

    result.push_back({0,-1});
    result.push_back({0,-2});
    result.push_back({1,-1});
    result.push_back({-1,-1});

    return result;
}

std::vector<Board::Dimensions> const&
Board::pawn_directions_dark()
{
    static std::vector<Board::Dimensions> result;

    result.push_back({0, 1});
    result.push_back({0, 2});
    result.push_back({1, 1});
    result.push_back({-1, 1});

    return result;
}

//Takes in a vector of directions of travel, and modifies them based
//on the current state of the board. returns a new vector.
// - if the pawn is in its initial row, adds {0,2} or {0,-2} to list
// - if pawn is near a piece that it can take, add diagonal to list

std::vector<Board::Dimensions>
Board::modify_pawn_dirs(Piece p, std::vector<Dimensions> dirs_travel)
{
    std::vector<Board::Dimensions> result;

    Position temp_posn = p.get_posn() + dirs_travel[0];
    if (!(temp_posn.x < 0 || temp_posn.x >= dimensions().width
        || temp_posn.y < 0 || temp_posn.y >= dimensions().height)) {

        if (operator[](temp_posn).get_piece_type() == Piece_type::null) {
            result.push_back(dirs_travel[0]);

            //checking for space 2 in front is only valid if space 1 is free
            temp_posn = p.get_posn() + dirs_travel[1];
            int curr_y = p.get_posn().y;

            if ( ((p.get_player() == Player::black && curr_y == 1)
                   || (p.get_player() == Player::white && curr_y == 6))
                   && operator[](temp_posn).get_piece_type() == Piece_type::null
                   ){
                result.push_back(dirs_travel[1]);
            }
        }
    }

    temp_posn = p.get_posn() + dirs_travel[2];
    if (!(temp_posn.x < 0 || temp_posn.x >= dimensions().width
        || temp_posn.y < 0 || temp_posn.y >= dimensions().height)) {

        if (operator[](temp_posn).get_piece_type() != Piece_type::null) {
            result.push_back(dirs_travel[2]);
        }
    }

    temp_posn = p.get_posn() + dirs_travel[3];
    if (!(temp_posn.x < 0 || temp_posn.x >= dimensions().width
           || temp_posn.y < 0 || temp_posn.y >= dimensions().height)) {

        if (operator[](temp_posn).get_piece_type() != Piece_type::null){
            result.push_back(dirs_travel[3]);
        }
    }

    return result;
}


Board::Rectangle
Board::all_positions() const
{
    return Rectangle::from_top_left(the_origin, dims_);
}

//if there's a white piece at the given position, return that
//piece. if there's a black piece, same. else, return a piece that
//doesn't mean anything
Piece
Board::get_piece_(Position pos) const
{
    Piece white_piece = white_.get_piece_from_set(pos);
    Piece black_piece = black_.get_piece_from_set(pos);

    if (white_piece.get_piece_type() != Piece_type::null) {
        return white_piece;
    } else if (black_piece.get_piece_type() != Piece_type::null) {
        return black_piece;
    } else {
        return Piece{Piece_type::null, Player::null, Position{0, 0}};
    }
}


void
Board::bounds_check_(Position pos) const
{
    if (!good_position(pos)) {
        throw Client_logic_error("Board: position out of bounds");
    }
}

bool
operator==(Board const& b1, Board const& b2)
{
    //two boards are equal if all their member data is equal
    return (b1.dims_ == b2.dims_ && b1.black_ == b2.black_
            && b1.white_ == b2.white_)   ;
}

bool
operator!=(Board const& b1, Board const& b2)
{
    return !(b1 == b2);
}

//Sets the piece type on board by modifying the appropriate Piece_set.
//Used primarily for pawn promotion to change type to "queen".
void Board::set_piece_as(Piece piece)
{
    Position start = piece.get_posn();
    if (piece.get_player() == Player::white) {
        white_.change_piece_as(start);
    } else if (piece.get_player() == Player::black) {
        black_.change_piece_as(start);
    } else {
        throw Client_logic_error("board::set_piece_as: cannot"
                                 " promote");
    }
}



