#include "board.hxx"

#include <algorithm>

using namespace ge211;

Board::Board(Dimensions dims)
        : dims_(dims),
        light_(white_start_moves),
        dark_(black_start_moves)

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

Board::reference
Board::operator[](Position pos)
{
    bounds_check_(pos);
    return reference(*this, pos);
}

// size_t
// Board::count_player(Player player) const
// {
//     switch (player) {
//     case Player::light:
//         return light_.size();
//     case Player::dark:
//         return dark_.size();
//     default:
//         return dims_.width * dims_.height -
//                light_.size() - dark_.size();
//     }
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

Board::Rectangle
Board::all_positions() const
{
    return Rectangle::from_top_left(the_origin, dims_);
}

// bool
// operator==(Board const& b1, Board const& b2)
// {
//     return b1.dims_ == b2.dims_ &&
//            b1.light_ == b2.light_ &&
//            b1.dark_ == b2.dark_;
// }

//used primarily for getting the color of the piece at this
//current position
// Player
// Board::get_(Position pos) const
// {
//     // if (dark_[pos]) {
//     //     return Player::dark;
//     // } else if (light_[pos]) {
//     //     return Player::light;
//     // } else {
//     //     return Player::neither;
//     //}
// }

Piece
Board::get_piece_(Position pos) const
{
    // Piece white_piece = light_.get(pos);
    // Piece& black_piece = dark_.get(pos);


    return Piece{Piece_type::null, Player::dark, Position{0,0}};
}

void
Board::set_(Position pos, Player player)
{
    //switch (player) {
    //case Player::dark:
    //     dark_[pos] = true;
    //     light_[pos] = false;
    //     break;
    //
    //case Player::light:
    //     dark_[pos] = false;
    //     light_[pos] = true;
    //     break;
    //
    // default:
    //     dark_[pos] = false;
    //     light_[pos] = false;
    //}
}


void
Board::bounds_check_(Position pos) const
{
    if (!good_position(pos)) {
        throw Client_logic_error("Board: position out of bounds");
    }
}

bool
operator!=(Board const& b1, Board const& b2)
{
    return !(b1 == b2);
}

// std::ostream&
// operator<<(std::ostream& os, Board const& board)
// {
//     Board::Dimensions dims = board.dimensions();
//
//     for (int y = 0; y < dims.height; ++y) {
//         for (int x = 0; x < dims.width; ++x) {
//             os << board[{x, y}];
//         }
//         os << "\n";
//     }
//
//     return os;
// }

// Board::reference::reference(Board& board, Piece piece) noexcept
//         : board_(board),
//           piece_(piece)
// { }

// Board::reference&
// Board::reference::operator=(reference const& that) noexcept
// {
//     *this = Piece(that);
//     return *this;
// }

// Board::reference&
// Board::reference::operator=(Player player) noexcept
// {
//     board_.set_(piece_, player);
//     return *this;
// }


// Board::reference::operator Player() const noexcept
// {
//     return board_.get_(piece_);
// }

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



