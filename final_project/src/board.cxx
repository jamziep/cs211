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


//for debug
// Piece piece(Piece_type::rook,Player::dark,{0,0});
// Piece_set pieces({piece});

Board::Board(Dimensions dims)
        : dims_(dims),
        white_(white_start_moves),
        black_(black_start_moves)
        // light_(pieces),
        // dark_(pieces)
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

// Player
// Board::operator[](Position pos) const
// {
//     bounds_check_(pos);
//     return get_(pos);
// }

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

    //find the starting position of piece
    Position start = p.get_posn();

    //modify the appropriate Piece_set
    if (p.get_player() == Player::white) {
        white_.change_posn(start, end);
    } else if (p.get_player() == Player::black) {
        black_.change_posn(start, end);
    } else {
        throw Client_logic_error("Model::change_piece_posn: cannot"
                                 "change the position of Player::neither");
    }


}

//find the king in either of the piece_sets, and return its location
//commented out for now, but KEEP WORKING on it

Position Board::find_king_location(Player p)
{
    //decide which piece_set to iterate through
    if (p == Player::black) {
        //iterate through the piece_set and try to find a piece that has
        //piece_type king
        Piece_set pcset = black_;
        for (size_t ii = 0; ii < pcset.size(); ++ii) {
            Piece curr_piece = pcset[ii];
            if (curr_piece.get_piece_type() == Piece_type::king) {
                //return the position of the king
                return curr_piece.get_posn();
            }
        }

        //if no king is found, that's a problem
        throw Client_logic_error("Board::find_king_location: no king"
                                 "found for dark");

    } else if (p == Player::white) {
        //iterate through the piece_set and try to find a piece that has
        //piece_type king
        Piece_set pcset = white_;
        for (size_t ii = 0; ii < pcset.size(); ++ii) {
            Piece curr_piece = pcset[ii];
            if (curr_piece.get_piece_type() == Piece_type::king) {
                //return the position of the king
                return curr_piece.get_posn();
            }
        }

        //if no king is found, that's a problem
        throw Client_logic_error("Board::find_king_location: no king"
                                 "found for light");

    } else {
        throw Client_logic_error("Board::find_king_location: can't find"
                                 "a king for a non black/white player");
    }
}

void Board::remove_by_posn(Position posn) {

    //first, find a piece that matches this posn
    Piece piece = operator[](posn);

    if (piece.get_player() == Player::white) {
        black_.remove(piece);
    } else if (piece.get_player() == Player::black) {
        white_.remove(piece);
    } else {
        throw Client_logic_error("Board::remove_by_posn: can't remove"
                                 "a player of type 'neither'");
    }
}

// Board::reference
// Board::operator[](Position pos)
// {
//     bounds_check_(pos);
//     return reference(*this, pos);
// }

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
    //modify this so it uses only the diagonals
    //use an AND: both x and y must exist
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

    //ses just the forward positions
    //and the two diagonal captures as directions of travel
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

    //uses just the forward direction
    //and the two diagonal captures as directions of travel
    result.push_back({0, 1});
    result.push_back({0, 2});
    result.push_back({1, 1});
    result.push_back({-1, 1});

    return result;
}

//takes in a vector of directions of travel, and modifies them based
//on the current state of the board. modifies in place based on reference
// - if the pawn is in its initial row, adds {0,2} or {0,-2} to list
// - if pawn is near a piece that it can take, add diagonal to list
void
Board::modify_pawn_dirs(Piece p, std::vector<Dimensions>& dirs_travel)
{
    //if there isn't a piece in front of the pawn in the diagonal directions,
    //assume the pawn can't move there. this could be modified for en passant
    Position temp_posn = p.get_posn() + dirs_travel[3];

    //need to do manual bounds check before calling operator[]
    if (temp_posn.x < 0 || temp_posn.x >= dimensions().width
           || temp_posn.y < 0 || temp_posn.y >= dimensions().height) {
        dirs_travel.erase(dirs_travel.begin() + 3);

    //then, check the condition for this given square
    } else if (operator[](temp_posn).get_piece_type() == Piece_type::null){
        dirs_travel.erase(dirs_travel.begin() + 3);
    }


    temp_posn = p.get_posn() + dirs_travel[2];
    if (temp_posn.x < 0 || temp_posn.x >= dimensions().width
        || temp_posn.y < 0 || temp_posn.y >= dimensions().height) {
        dirs_travel.erase(dirs_travel.begin() + 2);

    } else if (operator[](temp_posn).get_piece_type() == Piece_type::null) {
        dirs_travel.erase(dirs_travel.begin() + 2);
    }

    //if the pawn is not in its starting row, remove the second element from
    //vector as the pawn can't move two spaces otherwise
    int curr_y = p.get_posn().y;
    if ( (p.get_player() == Player::black && curr_y != 1)
           || (p.get_player() == Player::white && curr_y != 6) ){
        dirs_travel.erase(dirs_travel.begin() + 1);
    }

    //if there IS a piece in front of the pawn, can't move forward,
    //so remove the first element from vector
    temp_posn = p.get_posn() + dirs_travel[0];
    if (temp_posn.x < 0 || temp_posn.x >= dimensions().width
        || temp_posn.y < 0 || temp_posn.y >= dimensions().height) {
        dirs_travel.erase(dirs_travel.begin());

    } else if (operator[](temp_posn).get_piece_type() != Piece_type::null) {
        dirs_travel.erase(dirs_travel.begin());
    }
}


Board::Rectangle
Board::all_positions() const
{
    return Rectangle::from_top_left(the_origin, dims_);
}

Piece
Board::get_piece_(Position pos) const
{
    //figure this shit out, bc it's really a necessary step
    //in order to get pieces out of board

    Piece white_piece = white_.get_piece_from_set(pos);
    Piece black_piece = black_.get_piece_from_set(pos);

    //if there's a white piece at the given position, return that
    //piece. if there's a black piece, same. else, return a piece that
    //doesn't mean anything


    if (white_piece.get_piece_type() != Piece_type::null) {
        return white_piece;
    } else if (black_piece.get_piece_type() != Piece_type::null) {
        return black_piece;
    } else {
        return Piece{Piece_type::null, Player::black, Position{0, 0}};
    }
}

void
Board::set_(Position pos, Player player)
{

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

Board::multi_reference
Board::at_set(Position_set pos_set)
{
    return multi_reference(*this, pos_set);
}

Board::multi_reference::multi_reference(
        Board& board,
        Position_set pos_set) noexcept
        : board_(board),
          pos_set_(pos_set)
{ }



