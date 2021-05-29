#include "board.hxx"

#include <algorithm>

using namespace ge211;

//for debug
// Piece piece(Piece_type::rook,Player::dark,{0,0});
// Piece_set pieces({piece});

Board::Board(Dimensions dims)
        : dims_(dims),
        light_(white_start_moves),
        dark_(black_start_moves)
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
Board::operator[](Position pos)// const
{
    bounds_check_(pos);
    return get_piece_(pos);
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

    //ses just the forward position
    //and the two diagonal captures as directions of travel
    result.push_back({0,-1});
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
    result.push_back({1, 1});
    result.push_back({-1, 1});

    return result;
}

Board::Rectangle
Board::all_positions() const
{
    return Rectangle::from_top_left(the_origin, dims_);
}

Piece
Board::get_piece_(Position pos)
{
    //figure this shit out, bc it's really a necessary step
    //in order to get pieces out of board

    Piece* white_piece_ptr = light_.get_piece_ptr(pos);
    Piece* black_piece_ptr = dark_.get_piece_ptr(pos);

    //if there's a white piece at the given position, return that
    //piece. if there's a black piece, same. else, return a piece that
    //doesn't mean anything
    if (white_piece_ptr) {
        return *white_piece_ptr;
    } else if (black_piece_ptr) {
        return *black_piece_ptr;
    } else {
        return Piece{Piece_type::null, Player::dark, Position{0, 0}};
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
    return (b1.dims_ == b2.dims_ && b1.dark_ == b2.dark_
            && b1.light_ == b2.light_)   ;
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



