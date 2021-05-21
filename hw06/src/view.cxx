#include "view.hxx"

// Convenient type aliases:
using Color = ge211::Color;
using Sprite_set = ge211::Sprite_set;

// You can change this or even determine it some other way:
static int const grid_size = 36;

//for determining size of board
static int const ball_radius = grid_size/2;
static Color board_color = Color(61,165,98);
static Color highlight_color = Color(200,20,20);

View::View(Model const& model)
        : model_(model),
        // You may want to add sprite initialization here
          black_tile(ball_radius, Color(0, 0, 0)),
          white_tile(ball_radius, Color(255, 255, 255)),
          gray_tile(ball_radius, Color(100, 100, 100)),
          plays(ball_radius, Color(0, 0, 255)),
          //board_sprite({8*2*ball_radius, 8*2*ball_radius}, board_color)
          board_sprite({grid_size*8,grid_size*8}, board_color),
          square_sprite({grid_size,grid_size}, highlight_color),
          text_sprite(), //initialize empty, then add text later

          //for previewing positions
          move_preview({{}})
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

        //finds the player at given position. see operator[] in model.hxx
        Player curr_player = View::model_[posn];

        //model only stores info about the position in terms of the board,
        //not the screen. ex: {1,2}, {4,3}. make new posns for screen
        Position screen_posn = {posn.x * grid_size, posn.y * grid_size};

        // check for winner first
        if (View::model_.winner() != Player::neither){

            Player curr_winner = View::model_.winner();
            // draw a gray tile on loser spots,
            // and draw winner back onto proper places
            if (View::model_[posn] == other_player(curr_winner)){
                set.add_sprite(View::gray_tile, screen_posn, 3);
            } else if (View::model_[posn] == curr_winner) {
                if (curr_winner == Player::dark) {
                    set.add_sprite(View::black_tile, screen_posn, 3);
                } else if (curr_winner == Player::light) {
                    set.add_sprite(View::white_tile, screen_posn, 3);
                }
            }
        //if there's no winner yet
        } else {
            //draw a tile of that color and at that position
            if (curr_player == Player::dark) {
                set.add_sprite(View::black_tile, screen_posn, 3);
            } else if (curr_player == Player::light) {
                set.add_sprite(View::white_tile, screen_posn, 3);
            }

            //if find_move() is not NULL with the given posn, highlight this
            //square showing you can move there
            if (model_.find_move(posn)) {
                set.add_sprite(square_sprite, screen_posn,3);
            }
        }
    }

    //if the contents of sprite are not empty, add it to screen
    //uses the bool operator
    if (text_sprite) {
        set.add_sprite(View::text_sprite, {0, 0},4);
    }

    //if the player is previewing a move, take the full list of tiles
    //overturned by that move, and draw all those tiles in gray to preview
    //i.e. if move_preview is not empty
    // if (move_preview.first != Position{NULL,NULL}) {
    //     //add a sprite for each thing in move_preview.second
    //     for (Position posn : move_preview.second) {
    //         Position screen_posn{posn.x * grid_size, posn.y*grid_size};
    //         set.add_sprite(gray_tile,screen_posn,4)
    //     }
    // }
    if (!move_preview.empty()){
        for (Position posn : move_preview) {
            Position screen_posn{posn.x * grid_size, posn.y*grid_size};
            set.add_sprite(gray_tile,screen_posn,4);
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

//helper function: modify the text stored within the text sprite
void View::update_text_box(std::string text)
{
    //make a new builder for text
    ge211::Font sans30{"sans.ttf", 30};
    ge211::Text_sprite::Builder text_builder(sans30);
    text_builder.color(ge211::Color(57,255,255));

    //add the text to our builder and reconfigure
    text_builder.message(text);
    View::text_sprite.reconfigure(text_builder);

}

//helper function to draw that takes in a move, and modifies the sprite set
void View::set_move_preview(Move const* movep, ge211::Sprite_set& set) {

    //if the move pointer is not null, add these sprites to set
    if (movep) {
        for (ge211::Posn<int> posn : movep -> second) {
            set.add_sprite(gray_tile, posn);
        }
    }
}

//trying this again by just editing the member data. now with just
//a position set, since that seems simpler
void View::set_move_preview2(Position_set pset)
{
    View::move_preview = pset;
}




//things to do in view:

//make a way to draw all the sprites in the sprite set. in this
//case, all the sprites are gonna be the tokens on the board

//make a way to initialize the board as a green background with gridlines
//at every row and column