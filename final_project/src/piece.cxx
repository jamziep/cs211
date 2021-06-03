#include "piece.hxx"


Piece::Piece(Piece_type ptype, Player aplayer, Position aposn)
    : type_(ptype), posn_(aposn), player_(aplayer), active_(true)
{}

std::ostream& operator<<(std::ostream& os, Piece p)
{
    switch (p.get_piece_type()) {
    case Piece_type::pawn:
        return os << "P";
    case Piece_type::knight:
        return os << "K";
    case Piece_type::bishop:
        return os << "B";
    case Piece_type::rook:
        return os << "R";
    case Piece_type::queen:
        return os << "Q";
    case Piece_type::king:
        return os << "K";
    default:
        return os << "_";
    }
}

bool
operator==(Piece thisone, Piece thatone) {

    return (thisone.player_ == thatone.player_ &&
            thisone.type_ == thatone.type_ &&
            thisone.posn_ == thatone.posn_ &&
            thisone.active_ == thatone.active_);
}

bool
operator!=(Piece thisone, Piece thatone) {
    return !(thisone == thatone);
}
