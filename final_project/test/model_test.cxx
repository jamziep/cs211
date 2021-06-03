#include "model.hxx"
#include <catch.hxx>



// Initial test of "play_move." Only tests this one function and acts as an
// initial test before the others. Tests the possible moves of a couple of
// pieces and plays a move and rechecks possible moves based on that one move.
TEST_CASE("Model::play_move")
{
    Model m = Model();

    CHECK(m.turn() == Player::white);
    CHECK(m.winner() == Player::neither);

    CHECK(m.find_move({2,6}));
    CHECK_FALSE(m.find_move({4,4}));

    Move const* bishop_movep = m.find_move({5,7});
    Move const* rook_movep = m.find_move({7,7});
    Move const* queen_movep = m.find_move({3,7});
    Move const* king_movep = m.find_move({4,7});

    CHECK(bishop_movep -> second.empty());
    CHECK(rook_movep -> second.empty());
    CHECK(queen_movep -> second.empty());
    CHECK(king_movep -> second.empty());

    CHECK_FALSE(m[{2,4}] == Piece(Piece_type::pawn, Player::white,
                            Position{2,4}));

    m.play_move({2,6}, {2,4});

    CHECK(m[{2,4}] == Piece(Piece_type::pawn, Player::white,
                                    Position{2,4}));

    CHECK(m.find_move({2,1}));
    CHECK_FALSE(m[{2,3}] == Piece(Piece_type::pawn, Player::black,
                                  Position{2,3}));

    m.play_move({2,1}, {2,3});
    CHECK(m[{2,3}] == Piece(Piece_type::pawn, Player::black,
                                  Position{2,3}));

    CHECK(m.find_move({2,4}));
}

// Another initial test before the 5 main tests. Checks specifically pawns and
// their movement after one move.
TEST_CASE("Pawn moves") {

    Model m = Model();
    CHECK(m.turn() == Player::white);
    CHECK(m.winner() == Player::neither);

    Move const* pawn_moves = m.find_move({2,6});
    CHECK(pawn_moves);

    CHECK(pawn_moves -> second[{2,5}]);
    CHECK(pawn_moves -> second[{2,4}]);
    CHECK_FALSE(pawn_moves -> second[{3,5}]);
    CHECK_FALSE(pawn_moves -> second[{1,5}]);

}

// special case tests:
// TEST CASE: PAWN PROMOTION (1/5)
// this case tests pawn promotion, a special pawn condition when the pawn
// reaches the back rank of the opponent. When the pawn reaches the back
// rank, we test to see if it has been properly promoted to a queen. It also
// tests pawn, knight, and queen movement and special pawn movement like
// starting with a two-square move and capturing pieces diagonally.
TEST_CASE("Pawn promotion"){

    Model m = Model();

    CHECK(m.turn() == Player::white);
    CHECK(m.winner() == Player::neither);

    Move const* white_pawn_moves = m.find_move({5,6});
    CHECK(white_pawn_moves);
    CHECK(white_pawn_moves -> second[{5,4}]);

    m.play_move({5,6},{5,4});

    Move const* black_pawn_moves = m.find_move({6,1});
    CHECK(black_pawn_moves);
    CHECK(black_pawn_moves -> second[{6,3}]);

    m.play_move({6,1},{6,3});

    white_pawn_moves = m.find_move({5,4});
    CHECK(white_pawn_moves -> second[{6,3}]);
    m.play_move({5,4},{6,3});
    m.play_move({6,0},{7,2});
    m.play_move({6,3},{6,2});
    m.play_move({7,2},{6,0});
    m.play_move({6,2},{6,1});
    m.play_move({6,0},{7,2});
    m.play_move({6,1},{6,0});

    m.play_move({1,1},{1,2}); // b7 -> b6

    Move const* promoted = m.find_move({6,0});

    CHECK(promoted -> second[{6,5}]);
    CHECK(promoted -> second[{6,4}]);
    CHECK(promoted -> second[{5,1}]);
}

// TEST CASE: CASTLING KING SIDE (2/5)
// this case will test castling, which is a special type of king-rook
// movement. Castling is a special move if there is no pieces between the
// king and the rook in the staring position. If all checks for castling are
// met, the king will move to a better defended position (In this case, g1).
// The rook will then move to the space adjacent to the king. This test
// checks this special case and also checks king and rook movement, along
// with basic gameplay.
// White moves (black does repeated knight moves):
// e4
// Bc4
// Kf3
// Castles 0-0

TEST_CASE("castling king-side on white")
{

    Model m = Model();

    CHECK(m.turn() == Player::white);
    CHECK(m.winner() == Player::neither);

    Move const* white_moves = m.find_move({4,6});
    CHECK(white_moves -> second[{4,4}]);
    m.play_move({4,6},{4,4});
    m.play_move({6,0},{7,2});

    white_moves = m.find_move({5,7});
    CHECK(white_moves -> second[{2,4}]);
    m.play_move({5,7}, {2,4});
    m.play_move({7,2},{6,0});
    m.play_move({6,7}, {5,5});
    m.play_move({6,0},{7,2});

    m.play_move({4,7},{6,7});

    CHECK(m.return_piece_type({6,7}) == Piece_type::king);
    CHECK(m.return_piece_type({5,7}) == Piece_type::rook);
}

// TEST CASE: SCHOLAR'S MATE (3/5)
// this test will walk through an entire game of chess, testing pawn,
// knight, bishop, and queen movement as well as checkmate and check. This
// four-move game is an easy way to test an entire game and important
// interactions between the players in a possible game.
// The moves in this test are the following:
// 1. e4 - e5
// 2. Bc4 - Nc6
// 3. Qh5 - Nf6
// 4. Qxf7 - MATE

TEST_CASE("Scholar's Mate: White")
{

    Model m = Model();

    CHECK(m.turn() == Player::white);
    CHECK(m.winner() == Player::neither);

    m.play_move({4,6},{4,4}); // e4
    m.play_move({4,1},{4,3}); // e5
    m.play_move({5,7},{2,4}); // Bc4
    m.play_move({1,0},{2,2}); // Nc6
    m.play_move({3,7},{7,3}); // Qh5
    m.play_move({6,0},{5,2}); // Nf6
    m.play_move({7,3},{5,1}); // Qxf7 and mate

    CHECK(m.winner() == Player::white);
}

// TEST CASE: SAM LOYD STALEMATE (4/5)
// This test case will also play through an entire game of chess to test a
// stalemate endgame. This chess composition is the quickest way to stalemate
// in a game of chess and allows us to test check and stalemate functionality
// Stalemate is an important endgame because it is a draw between the players.
// It occurs when it is one player's turn but has no legal moves. This will
// test stalemate, piece movement and captures, and check (but not checkmate).
// The composition:
// 1. e3 - a5
// 2. Qh5 - Ra6
// 3. Qxa5 - h5
// 4. h4 - Rah6
// 5. Qxc7 - f6
// 6. Qxd7+ - kf7
// 7. Qxb7 - Qd3
// 8. Qxb8 - Qh7
// 9. Qxc8 - Kg6
// 10. Qe6 - Stalemate

TEST_CASE("Sam Loyd Stalemate")
{
    Model m = Model();

    CHECK(m.turn() == Player::white);
    CHECK(m.winner() == Player::neither);

    m.play_move({4, 6}, {4, 5});
    m.play_move({0, 1}, {0, 3});
    m.play_move({3, 7}, {7, 3});
    m.play_move({0, 0}, {0, 2});
    m.play_move({7, 3}, {0, 3});
    m.play_move({7, 1}, {7, 3});
    m.play_move({7, 6}, {7, 4});
    m.play_move({0, 2}, {7, 2});
    m.play_move({0, 3}, {2, 1});
    m.play_move({5, 1}, {5, 2});
    m.play_move({2, 1}, {3, 1});

    Move const* empty_moves = m.find_move({5,2});
    CHECK(empty_moves -> second.empty());

    m.play_move({4, 0}, {5, 1});
    m.play_move({3, 1}, {1, 1});
    m.play_move({3, 0}, {3, 5});
    m.play_move({1, 1}, {1, 0});
    m.play_move({3, 5}, {7, 1});
    m.play_move({1, 0}, {2, 0});
    m.play_move({5, 1}, {6, 2});
    m.play_move({2, 0}, {4, 2});

    auto king_ = m.return_piece_type({6,2});
    CHECK(king_ == Piece_type::king);
    CHECK(m.winner() == Player::neither);
    CHECK(m.turn() == Player::neither);

}
// TEST CASE: ALL PIECE TYPES (5/5)
// tests the movement of all pieces. Explicitly checks for moves for all
// Piece_types: pawn, rook, bishop, queen, knight, king. Will also test a
// special move case for pawn, where if a pawn is in its starting position,
// it can move an extra square.

TEST_CASE("Movement of all pieces")
{
    Model m = Model();

    CHECK(m.turn() == Player::white);
    CHECK(m.winner() == Player::neither);

    Move const* white_pawn = m.find_move({4,6});
    CHECK(white_pawn -> second[{4,4}]);
    CHECK(white_pawn -> second[{4,5}]);

    Move const* white_knight = m.find_move({1,7});
    CHECK(white_knight -> second[{0,5}]);
    CHECK(white_knight -> second[{2,5}]);

    m.play_move({4,6},{4,4});
    m.play_move({1,1},{1,3});

    Move const* white_bishop = m.find_move({5,7});
    CHECK(white_bishop -> second[{4,6}]);
    CHECK(white_bishop -> second[{3,5}]);

    Move const* white_king = m.find_move({4,7});
    CHECK(white_king -> second[{4,6}]);

    Move const* white_queen = m.find_move({3,7});
    CHECK(white_queen -> second[{4,6}]);
    CHECK(white_queen -> second[{5,5}]);

    white_pawn = m.find_move({4,4});
    CHECK(white_pawn -> second[{4,3}]);
    CHECK(!white_pawn -> second[{4,2}]);
}


