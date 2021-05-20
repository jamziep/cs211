#include "model.hxx"
#include <catch.hxx>

//std::includes(array, array.begin(), array.end())


using namespace ge211;

// These pass with the starter code and should continue
// to pass.
TEST_CASE("Passes with starter code")
{
    Model m(8, 8);
    CHECK( m.board() == Model::Rectangle{0, 0, 8, 8} );
    CHECK_FALSE( m.is_game_over() );
    CHECK( m.turn() == Player::dark );
    CHECK( m.winner() == Player::neither );
    CHECK( m[{0, 0}] == Player::neither );
    CHECK( m.find_move({0, 0}) == nullptr );
    CHECK_THROWS_AS( m.play_move({0, 0}), Client_logic_error );
}

// This fails with the starter code, but should pass.
TEST_CASE("Fails with starter code")
{
    Model m(6);
    CHECK( m.find_move({2, 2}) );
}

// This fails with the starter code, but should pass.
TEST_CASE("Throws with starter code")
{
    Model m(4);
    m.play_move({1, 1});
}

// YOU NEED MANY MORE TESTS!

//things to test:

//test cases:
//next moves relative to a board that has been created
//flips caused by each of the possible next moves
//winner is properly assigned when board is full
//winner is properly assigned when neither player has any more moves
//when one player has no more moves, changes to other player

//functions to test:
//play_move
// - when it's game over
// - when invalid move happens
// - changes the player
//find_flips
//evaluate_position
//compute_next_moves
//set_game_over



