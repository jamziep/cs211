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

//for accessing the board during testing
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

        for (const auto& move : model.next_moves_){
            ge211::Posn<int> pos = move.first;
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
    Player get_winner()
    {
        return model.winner_;
    }
};

// test cases for playing moves and valid moves.
TEST_CASE("Play the game")
{
    Model model(4);
    Test_access t{model};

    // test add pieces to the center.
    model.play_move({1,1});
    model.play_move({1,2});
    model.play_move({2,2});
    model.play_move({2,1});
    // find valid moves based on this.
    CHECK(model.find_move({0,2}));
    CHECK(model.find_move({1,3}));
}

// find the winner using play move
TEST_CASE("black loses")
{
    Model model(2);
    // play all possible moves:
    model.play_move({0,0});
    model.play_move({0,1});
    model.play_move({1,0});
    model.play_move({1,1});
    // find the winner.

    CHECK(model.winner() == Player::neither);
}

// checks for invalid move using play move
TEST_CASE("bad moves")
{
    Model model(4);
    CHECK_THROWS_AS(model.play_move({0,0}),Client_logic_error);
}

// using play move check that players properly switch
TEST_CASE("check for changing players")
{
    Model model(4);
    model.play_move({1, 1});
    CHECK(model.turn() == Player::light);
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
// works when we get rid of center 4 tiles
TEST_CASE("Possible moves on the board")
{
    Model model(4);
    Test_access t{model};
    Position_set f;

    // start with the center 4
    f = t.compute_next();
    CHECK(f == Position_set{{1,1}, {1,2}, {2,1}, {2,2}});
    // fill center:
    t.board()[{1,1}] = Player::dark;
    t.board()[{1,2}] = Player::light;
    t.board()[{2,2}] = Player::dark;
    t.board()[{2,1}] = Player::light;
    f.clear();
    f = t.compute_next();
    CHECK(f == Position_set{{0,2}, {1,3}, {2,0}, {3,1}});
    // white only:
    t.board()[{1,1}] = Player::light;
    t.board()[{1,2}] = Player::light;
    t.board()[{2,2}] = Player::light;
    t.board()[{2,1}] = Player::light;
    f.clear();
    f = t.compute_next();
    CHECK(f.empty());
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

//Carries out some of the tests we did visually, by interacting
// with the board, at the start of this code process. Figured
// it might be wise to show them in our test cases as well
TEST_CASE("Early functionality checks") {

    //initialize board
    Model model(6,4);
    Test_access t{model};

    //test that turn changes properly after each turn
    // current player is black
    model.play_move({2,1});
    CHECK(model.turn() == Player::light);
    model.play_move({3,1});
    CHECK(model.turn() == Player::dark);
    model.play_move({3,2});
    CHECK(model.turn() == Player::light);
    model.play_move({2,2});
    CHECK(model.turn() == Player::dark);

    //check that a valid move registers as such
    //current player is black
    CHECK(model.find_move({3,0}));
    CHECK(model.find_move({4,1}));
    CHECK(model.find_move({1,2}));
    CHECK(model.find_move({2,3}));

    //bounds checking: make sure playing positions on the outer
    //limits of the board doesn't cause out-of-bounds errors
    t.board()[{4,1}] = Player::light;
    t.board()[{1,1}] = Player::light;

    Position_set pset;

    //valid directions of travel
    pset = t.find_flips({0,1},{1,0});
    CHECK_FALSE(pset.empty());
    pset = t.find_flips({5,1},{-1,0});
    CHECK_FALSE(pset.empty());

    //ensure that the position set from evaluate_position includes all
    //the things that flip AND the cell itself
    pset = t.evaluate_position({5,1});
    Position_set test_pset{{3,1},{4,1},{5,1}};
    CHECK(pset == test_pset);

    //invalid directions of travel, shouldn't crash
    pset = t.find_flips({5,1},{1,0});
    CHECK(pset.empty());
    pset = t.find_flips({0,1},{-1,0});
    CHECK(pset.empty());

    //ensure that model doesn't let you play in a position that is
    //already occupied
    pset = t.evaluate_position({2,1});
    CHECK(pset.empty());

    //make sure that next_moves_ refills properly after a turn
    //means light plays are added and dark plays are removed
    model.play_move({3,0});
    CHECK(model.turn() == Player::light);
    CHECK(model.find_move({2,0}));
    CHECK(model.find_move({4,0}));
    CHECK(model.find_move({4,2}));

    CHECK_FALSE(model.find_move({5,0}));
    CHECK_FALSE(model.find_move({5,1}));
    CHECK_FALSE(model.find_move({1,2}));
}


