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

// special case tests:
TEST_CASE("Pawn promotion"){
    // initialize model
    // this case tests pawn promotion, a special pawn condition when the pawn
    // reaches the back rank of the opponent. It also tests pawn, knight, and
    // queen movement and special pawn movement like starting with a
    // two-square move and capturing pieces diagonally.
    Model m = Model();

    //check that model initialized properly
    CHECK(m.turn() == Player::white);
    CHECK(m.winner() == Player::neither);

    // check moves that exist for the white pawn to be promoted.
    Move const* white_pawn_moves = m.find_move({5,6});
    CHECK(white_pawn_moves);
    CHECK(white_pawn_moves -> second[{5,4}]);
    // play the move. (pawn to f4)
    m.play_move({5,6},{5,4});

    // check moves for the black pawn to be taken.
    Move const* black_pawn_moves = m.find_move({6,1});
    CHECK(black_pawn_moves);
    CHECK(black_pawn_moves -> second[{6,3}]);
    // play move. (black pawn to g5)
    m.play_move({6,1},{6,3});

    // sequence:
    // white pawn takes black pawn on g5.
    white_pawn_moves = m.find_move({5,4});
    CHECK(white_pawn_moves -> second[{6,3}]);
    m.play_move({5,4},{6,3});
    // repeat knight to h6 over and over as pawn moves towards back rank.
    m.play_move({6,0},{7,2}); //knight
    m.play_move({6,3},{6,2}); //pawn
    m.play_move({7,2},{6,0}); //knight
    m.play_move({6,2},{6,1}); //pawn
    m.play_move({6,0},{7,2}); //knight
    m.play_move({6,1},{6,0}); //pawn ----> queen
    // move a random black pawn to switch sides back to white.
    m.play_move({1,1},{1,2}); // b7 -> b6
    // check to see if the pawn has properly promoted to queen.
    Move const* promoted = m.find_move({6,0});
    // these should all be valid if the pawn was promoted properly.
    CHECK(promoted -> second[{6,5}]);
    CHECK(promoted -> second[{6,4}]);
    CHECK(promoted -> second[{5,1}]);
}

TEST_CASE("castling king-side on white")
{
    // this case will test castling, which is a special type of king-rook
    // movement. This will test king and rook movement, as well as special
    // exception rules in chess.
    // initialize model
    Model m = Model();

    //check that model initialized properly
    CHECK(m.turn() == Player::white);
    CHECK(m.winner() == Player::neither);

    // white moves (black does knight repeats)
    // e2 -> e4
    // Bf1 -> c4
    // Kg1 -> f3
    // Castle 0-0
    Move const* white_moves = m.find_move({4,6});
    CHECK(white_moves -> second[{4,4}]);
    m.play_move({4,6},{4,4}); // e2 -> e4
    m.play_move({6,0},{7,2}); //black knight repeats
    // check bishop
    white_moves = m.find_move({5,7});
    CHECK(white_moves -> second[{2,4}]);
    m.play_move({5,7}, {2,4}); //Bf1 -> c4
    m.play_move({7,2},{6,0}); //knight repeats
    m.play_move({6,7}, {5,5}); //Kg1 -> f3
    m.play_move({6,0},{7,2}); //knight repeats
    // castle kingside:
    m.play_move({4,7},{6,7});
    // check if castling was completed properly:
    CHECK(m.return_piece_type({6,7}) == Piece_type::king);
    CHECK(m.return_piece_type({5,7}) == Piece_type::rook);
}

TEST_CASE("Scholar's Mate: White")
{
    // this test will walk through an entire game of chess, testing pawn,
    // knight, bishop, and queen movement as well as checkmate.
    // initialize model
    Model m = Model();

    //check that model initialized properly
    CHECK(m.turn() == Player::white);
    CHECK(m.winner() == Player::neither);

    // scholar's mate:
    // 1. e4 - e5
    // 2. Bc4 - Nc6
    // 3. Qh5 - Nf6
    // 4. Qxf7 - MATE
    m.play_move({4,6},{4,4}); // e4
    m.play_move({4,1},{4,3}); // e5
    m.play_move({5,7},{2,4}); // Bc4
    m.play_move({1,0},{2,2}); // Nc6
    m.play_move({3,7},{7,3}); // Qh5
    m.play_move({6,0},{5,2}); // Nf6
    m.play_move({7,3},{5,1}); // Qxf7 and mate

    // this should be white with checkmate.
    CHECK(m.winner() == Player::white);
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