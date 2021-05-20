#include "view.hxx"

// Convenient type aliases:
using Color = ge211::Color;
using Sprite_set = ge211::Sprite_set;

// You can change this or even determine it some other way:
static int const grid_size = 36;

//for determining size of board
static int const ball_radius = grid_size/2;
static Color board_color = Color(61,165,98);

View::View(Model const& model)
        : model_(model),
        // You may want to add sprite initialization here
          black_tile(ball_radius, Color(0, 0, 0)),
          white_tile(ball_radius, Color(255, 255, 255)),
          gray_tile(ball_radius, Color(100, 100, 100)),
          //board_sprite({8*2*ball_radius, 8*2*ball_radius}, board_color)
          board_sprite({grid_size*8,grid_size*8}, board_color),
          text_sprite() //initialize empty, then add text later
{
    // //using the syntax from the window creation function:
    // const ge211::Rectangle_sprite board_sprite = ge211::Rectangle_sprite
    //         (grid_size * model_.board().dimensions(), board_color);
}

void View::draw(Sprite_set& set)
{
    //draw the board sprite, a rectangle located at 0,0
    set.add_sprite(View::board_sprite, ge211::Posn<int>{0,0} );

    //iterate through the list of black and white tiles, and display.
    //goes into the "model" attribute of "View" and returns
    //model_.all_positions(). See model.hxx and board.hxx
    for (Position posn : View::model_.board()) {
       // if (View::model_[posn] == Player::neither) {
       //     continue;
       // }

        //finds the player at given position. see operator[] in model.hxx
        Player curr_player = View::model_[posn];

        //model only stores info about the position in terms of the board,
        //not the screen. ex: {1,2}, {4,3}. make new posns for screen
        Position screen_posn = {posn.x * grid_size, posn.y * grid_size};

        // check for winner first
        if (View::model_.winner() != Player::neither){
            auto curr_winner = View::model_.winner();
            Position place = View::Position(0, 0);
            // iterate over entire board and draw a gray tile on loser spots,
            // and draw winner back onto proper places
            for (auto pos : View::model_.board()){
                if (View::model_[pos] == other_player(curr_winner)){
                    place = {pos.x * grid_size, pos.y * grid_size};
                    set.add_sprite(View::gray_tile, place, 1);
                } else if (View::model_[pos] == curr_winner) {
                    place = {pos.x * grid_size, pos.y * grid_size};
                    if (curr_winner == Player::dark){
                        set.add_sprite(View::black_tile, place, 1);
                    } else if (curr_winner == Player::light)
                        set.add_sprite(View::white_tile, place, 1);
                }
            }
        } else {
            //draw a tile of that color and at that position
            if (curr_player == Player::dark) {
                set.add_sprite(View::black_tile, screen_posn, 3);
            } else if (curr_player == Player::light) {
                set.add_sprite(View::white_tile, screen_posn, 3);
            }
        }
    }

    //if the contents of the sprite are not empty, add it to screen
    //uses the bool operator
    if (text_sprite) {
        set.add_sprite(View::text_sprite, {0, 0},4);
    }
}

View::Dimensions
View::initial_window_dimensions() const
{
    // You can change this if you want:
    return grid_size * model_.board().dimensions();
    //return Dimensions(1024, 768);
}

std::string
View::initial_window_title() const
{
    // You can change this if you want:
    return "Reversi";
}

//things to do in view:

//make a way to draw all the sprites in the sprite set. in this
//case, all the sprites are gonna be the tokens on the board

//make a way to initialize the board as a green background with gridlines
//at every row and column