#include "controller.hxx"

//for visual debug
#include <iostream>

//copied this value from view.cxx for finding size of board
//ideally I'd like to include the entire view.cxx file, since
//these controls depend on the visual board dims
static int const grid_size = 36;
static int const ball_radius = grid_size/2;

Controller::Controller(int size)
        : Controller(size, size)
{ }

Controller::Controller(int width, int height)
        : model_(width, height),
          view_(model_)
{ }

void
Controller::draw(ge211::Sprite_set& sprites)
{
    view_.draw(sprites);
}

View::Dimensions
Controller::initial_window_dimensions() const
{
    return view_.initial_window_dimensions();
}

std::string
Controller::initial_window_title() const
{
    return view_.initial_window_title();
}

//if key pressed is q, quit game
void Controller::on_key(ge211::Key key) {
    if (key == ge211::Key::code('q')) {
        quit();
    }
}

//on mouse down, try to place a token at the position in board
//that's closest to that mouse click. iterate through all the squares
// of the board and see what the closest square is.
void Controller::on_mouse_down(ge211::Mouse_button btn,
                               ge211::Posn<int> mouse_posn)
{
     //for all the positions currently occupied
    // in board, see all_positions() in model.hxx

    for (int col_ind = 0; col_ind < 8; ++col_ind) {
        for (int row_ind = 0; row_ind < 8; ++row_ind) {

            //current position we're checking is at (row, col)
            //a mouse click is inside the square given by this position
            //if its position is inside its bounding box
            int square_left = col_ind;
            int square_right = col_ind + 1;
            int square_top = row_ind;
            int square_bottom = row_ind + 1;

            //for converting between screen position and board index
            double mouse_x_board = mouse_posn.x / double(grid_size);
            double mouse_y_board = mouse_posn.y / double(grid_size);

            //ways the mouse click posn could be outside the bounding
            //box of this square: left of square_left, right of square_right,
            //above square_top, below square_bottom. if none of these are true,
            //mouse click is inside this current box

            if (!(mouse_x_board < square_left || mouse_x_board > square_right
            || mouse_y_board < square_top || mouse_y_board > square_bottom)) {

                //eventually this will need to incorporate checks as to
                //whether or not a move is valid
                Player turn = Controller::model_.turn();
                ge211::Posn<int> square_coords{col_ind, row_ind};


                //add a tile of the current player's color to the board,
                //by updating the state of the model
                try {
                    Controller::model_.play_move(square_coords);

                    //remove any existing text
                    ge211::Font sans30{"sans.ttf", 30};
                    ge211::Text_sprite::Builder text_builder(sans30);
                    ge211::Color TEXT_COLOR(255,0,0);
                    text_builder.message("");
                    view_.text_sprite.reconfigure(text_builder);

                } catch(ge211::Client_logic_error) {
                    //catch an invalid move error by printing "invalid move"
                    // to screen

                    //for now I'm remaking the whole builder in order to recon-
                    //figure the text sprite. could make this a member of
                    // something later
                    ge211::Font sans30{"sans.ttf", 30};
                    ge211::Text_sprite::Builder text_builder(sans30);
                    ge211::Color TEXT_COLOR(255,0,0);
                    text_builder.message("Invalid move");
                    view_.text_sprite.reconfigure(text_builder);
                }

                //now that we've found the closest square, break out
                return;
            }
        }
    }

    //if we got here, mouse click was not associated with any square
    return;
}


//if key pressed is q, exit out

//bonus functionality:

//this can be implemented in view as well. show all possible moves
//for the player by showing the outline of a token of that color
//in every square where the player can move. outline should be a
//border only

//on mouse move, show the possible moves for the player. if the
//player hovers over a valid position, make the outline slightly
//darker over that token

