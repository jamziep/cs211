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

    //{2,2} is the top left corner of the board. why would there be
    //a move possible there? you would think that this posn has al-
    //ready been played, so it wouldn't be possible at this point
}

// This fails with the starter code, but should pass.
TEST_CASE("Throws with starter code")
{
    Model m(4);
    m.play_move({1, 1});
}

// YOU NEED MANY MORE TESTS!

TEST_CASE("Asymmetric board") {

    //make a board that's not 8x8 and ensure all 4 center
    //positions are initialized right

    Model m(6,4);
    CHECK(m.board() == Model::Rectangle{0,0,6,4});

    //basic checks
    CHECK_FALSE( m.is_game_over() );
    CHECK( m.turn() == Player::dark );
    CHECK( m.winner() == Player::neither );
    CHECK( m[{0, 0}] == Player::neither );
    CHECK( m.find_move({0, 0}) == nullptr );

    //check that center four positions are properly initialized
    CHECK( m[{2,1}] == Player::dark );
    CHECK( m[{3,1}] == Player::light );
    CHECK( m[{2,2}] == Player::dark );
    CHECK( m[{3,2}] == Player::light );

    //check that valid moves




}



