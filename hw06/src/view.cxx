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
          //board_sprite({8*2*ball_radius, 8*2*ball_radius}, board_color)
          board_sprite({grid_size*8,grid_size*8}, board_color)
{
    // //using the syntax from the window creation function:
    // const ge211::Rectangle_sprite board_sprite = ge211::Rectangle_sprite
    //         (grid_size * model_.board().dimensions(), board_color);
}

void View::draw(Sprite_set& set)
{
    // TODO, PROBABLY

    //draw the board sprite, a rectangle located at 0,0
    set.add_sprite(View::board_sprite, ge211::Posn<int>{0,0} );

    //iterate through the list of black and white tiles, and display
    //goes into the "model" attribute of "View" and returns
    //model_.all_positions(). See model.hxx and board.hxx

    for (Position posn : View::model_.board()) {

        if (View::model_[posn] == Player::neither) {
            continue;
        }

        //finds the player at given position. see operator[] in model.hxx
        Player curr_player = View::model_[posn];

        //model only stores info about the position in terms of the board,
        //not the screen. ex: {1,2}, {4,3}. make new posns for screen
        Position screen_posn = {posn.x * grid_size, posn.y * grid_size};

        //draw a tile of that color and at that position
        if (curr_player == Player::dark) {
            set.add_sprite(View::black_tile, screen_posn);
        } else if (curr_player == Player::light) {
            set.add_sprite(View::white_tile, screen_posn);
        }
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