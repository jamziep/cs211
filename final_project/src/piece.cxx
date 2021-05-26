#include "piece.hxx"

std::ostream& operator<<(std::ostream& os, Piece p)
{
    switch (p.type) {
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

