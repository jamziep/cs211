#include "model.hxx"
#include <catch.hxx>

TEST_CASE("example test (TODO: replace this)")
{
    CHECK( 1 + 1 == 2 );
}

//
// TODO: Write preliminary model tests.
//
// These tests should demonstrate at least six of the functional
// requirements.
//

TEST_CASE("Model::play_move")
{
    //initialize a model
    Model m = Model();

    //check the possible moves. current player is white
    CHECK(m.turn() == Player::white);
    CHECK(m.winner() == Player::neither);

    //check that moves exist for a pawn, but that moves don't exist for
    //a random position in space
    CHECK(m.find_move({2,6}));
    CHECK_FALSE(m.find_move({4,4}));

    //bishops, rooks, and other blocked pieces also shouldn't have any valid
    // moves at this time
    Move const* bishop_movep = m.find_move({5,7});
    Move const* rook_movep = m.find_move({7,7});
    Move const* queen_movep = m.find_move({3,7});
    Move const* king_movep = m.find_move({4,7});

    CHECK(bishop_movep -> second.empty());
    CHECK(rook_movep -> second.empty());
    CHECK(queen_movep -> second.empty());
    CHECK(king_movep -> second.empty());

    //see what's at a position first
    CHECK_FALSE(m[{2,4}] == Piece(Piece_type::pawn, Player::white,
                            Position{2,4}));

    //try to play a move
    m.play_move({2,6}, {2,4});

    //check that the piece is now here
    CHECK(m[{2,4}] == Piece(Piece_type::pawn, Player::white,
                                    Position{2,4}));

    //now, try to play a move for black
    CHECK(m.find_move({2,1}));
    CHECK_FALSE(m[{2,3}] == Piece(Piece_type::pawn, Player::black,
                                  Position{2,3}));

    //make a move
    m.play_move({2,1}, {2,3});
    CHECK(m[{2,3}] == Piece(Piece_type::pawn, Player::black,
                                  Position{2,3}));

    //check that previous moves by white exist on the board
    CHECK(m.find_move({2,4}));
}

TEST_CASE("Pawn moves") {

    //initialize a model
    Model m = Model();

    //check that model initialized properly
    CHECK(m.turn() == Player::white);
    CHECK(m.winner() == Player::neither);

    //check that moves exist for a pawn
    Move const* pawn_moves = m.find_move({2,6});
    CHECK(pawn_moves);

    //check WHICH moves exist for a pawn. at this point the
    //pawn should only be able to move forward 1 or 2 spaces
    CHECK(pawn_moves -> second[{2,5}]);
    CHECK(pawn_moves -> second[{2,4}]);
    CHECK_FALSE(pawn_moves -> second[{3,5}]);
    CHECK_FALSE(pawn_moves -> second[{1,5}]);

}

TEST_CASE("Bishop moves")
{

}


TEST_CASE("Knight moves")
{

}

TEST_CASE("King moves")
{

}

TEST_CASE("Queen moves")
{

}

TEST_CASE("Rook moves")
{

}

TEST_CASE("White in check")
{


}

TEST_CASE("Black in checkmate")
{

}

TEST_CASE("White in stalemate")
{

}