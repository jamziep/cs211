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



}

