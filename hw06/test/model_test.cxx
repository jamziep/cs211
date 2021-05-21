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

struct Test_access
{
    Model& model;
    Board& board()
    {
        return model.board_;
    }

    Position_set
    find_flips(Model::Position p,
               Model::Dimensions d)
    {
        return model.find_flips_(p,d);
    }

    Position_set
    evaluate_position(Model::Position p)
    {
        return model.evaluate_position_(p);
    }

    // unlike, compute_next_moves_, compute_next will return a position set
    // with all possible moves given the board. This is for testing purposes.
    Position_set compute_next()
    {
        model.next_moves_.clear();
        model.compute_next_moves_();
        Position_set posns {};
        for (const auto & [pos, flips] : model.next_moves_){
                posns[pos] = true;
        }
        return posns;
    }

    //test_win will also return the player that wins, unlike the void
    // set_game_winner
    Player test_win()
    {
        model.set_game_over_();
        return model.winner_;
    }
};

// test cases for play game
TEST_CASE("Play the game")
{
    Model model;
}

// tests find_flips_ in model.cxx
TEST_CASE("flip check")
{
    Model model;
    Test_access t{model};

    t.board()[{2,2}] = Player::dark;
    t.board()[{2,3}] = Player::light;

    Position_set f;

    // simple flip check
    f = t.find_flips({2,4}, {0,1});
    CHECK(f.empty());
    f = t.find_flips({2,4}, {0, -1});
    CHECK(f == Position_set{{2,3}});
    // diagonal flip check
    f.clear();
    t.board()[{3,2}] = Player::dark;
    f = t.find_flips({1,4},{1,1});
    CHECK(f.empty());
    f = t.find_flips({1, 4}, {1, -1});
    CHECK(f == Position_set{{2,3}});
    //isolated tile
    f.clear();
    f = t.find_flips({4,4}, {1,1});
    f = t.find_flips({4,4}, {0,1});
    f = t.find_flips({4,4}, {1,0});
    f = t.find_flips({4,4}, {-1,-1});
    CHECK(f.empty());
    //multiple tiles
    f.clear();
    t.board()[{1,2}] = Player::dark;
    t.board()[{2,2}] = Player::light;
    t.board()[{3,2}] = Player::light;
    t.board()[{4,2}] = Player::light;
    f = t.find_flips({5,2}, {1,0});
    CHECK(f.empty());
    f = t.find_flips({5,2}, {-1,0});
    CHECK(f == Position_set{{2,2}, {3,2}, {4,2}});
}


// tests evaluate_position_ in model.cxx
TEST_CASE("evaluating positions")
{
    Model model;
    Test_access t{model};
    Position_set f;

    //initialize start positions
    t.board()[{4,4}] = Player::dark;
    t.board()[{5,4}] = Player::light;
    t.board()[{5,5}] = Player::dark;
    t.board()[{4,5}] = Player::light;
    f = t.evaluate_position({6,4});
    CHECK(f == Position_set{{6,4},{5,4}});
    f.clear();
    f = t.evaluate_position({4,6});
    CHECK(f == Position_set{{4,5}, {4,6}});

    // checking a position with no validity
    f.clear();
    f = t.evaluate_position({1,1});
    CHECK(f.empty());
}

// tests compute_next_moves_
// this should work when we get rid of the four tiles in the middle of the board
TEST_CASE("Possible moves on the board")
{
    Model model(4);
    Test_access t{model};
    Position_set f;

    // start with the center 4
    f = t.compute_next();
    CHECK(f == Position_set{{1,1}, {1,2}, {2,1}, {2,2}});
    


}

//tests the set_game_over function
TEST_CASE("winner test")
{
    Model model;
    Test_access t{model};
    Player curr_winner;
    // case 1: only white remains
    t.board()[{2,2}] = Player::light;
    t.board()[{3,2}] = Player::light;
    t.board()[{4,2}] = Player::light;
    curr_winner = t.test_win();
    CHECK(curr_winner == Player::light);
    // case 2: black by majority
    t.board()[{2,2}] = Player::dark;
    t.board()[{3,2}] = Player::dark;
    t.board()[{4,2}] = Player::light;
    curr_winner = t.test_win();
    CHECK(curr_winner == Player::dark);
    // case 3: tie game
    t.board()[{2,2}] = Player::dark;
    t.board()[{3,2}] = Player::dark;
    t.board()[{4,2}] = Player::light;
    t.board()[{5, 2}] = Player::light;
    curr_winner = t.test_win();
    CHECK(curr_winner == Player::neither);
}

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



