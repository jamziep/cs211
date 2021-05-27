//
// Created by seanp on 5/26/2021.
//

#include <vector>
#include "piece.hxx"

std::vector<Piece> black_start_moves{
    Piece(Piece_type::pawn, Player::dark,{0,6} ),
    Piece(Piece_type::pawn, Player::dark,{1,6} ),
    Piece(Piece_type::pawn, Player::dark,{2,6} ),
    Piece(Piece_type::pawn, Player::dark,{3,6} ),
    Piece(Piece_type::pawn, Player::dark,{4,6} ),
    Piece(Piece_type::pawn, Player::dark,{5,6} ),
    Piece(Piece_type::pawn, Player::dark,{6,6} ),
    Piece(Piece_type::pawn, Player::dark,{7,6} ),

    Piece(Piece_type::rook, Player::dark,{0,7} ),
    Piece(Piece_type::knight, Player::dark,{1,7} ),
    Piece(Piece_type::bishop, Player::dark,{2,7} ),
    Piece(Piece_type::king, Player::dark,{3,7} ),
    Piece(Piece_type::queen, Player::dark,{4,7} ),
    Piece(Piece_type::bishop, Player::dark,{5,7} ),
    Piece(Piece_type::knight, Player::dark,{6,7} ),
    Piece(Piece_type::rook, Player::dark,{7,7} ),
};