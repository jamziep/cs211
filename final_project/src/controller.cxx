#include "controller.hxx"

//copied this value from view.cxx for finding size of board
//ideally I'd like to include the entire view.cxx file, since
//these controls depend on the visual board dims
static int const grid_size = 36;

Controller::Controller()
        : model_(),
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

    //maybeTODO: de-hardcode this so that it can work w/ boards of
    //diff sizes

    for (int col_ind = 0; col_ind < 8; ++col_ind) {
        for (int row_ind = 0; row_ind < 8; ++row_ind) {

            if (Controller::mouse_is_within_square_(mouse_posn, col_ind,
                                                    row_ind)) {

                //eventually this will need to incorporate checks as to
                //whether or not a move is valid
                ge211::Posn<int> square_coords{col_ind, row_ind};

                //add a tile of the current player's color to the board,
                //by updating the state of the model

                //If result of find_move is not null, it's a valid move, so
                // play it. Else, interpret as an invalid move

                if (Controller::model_.find_move(square_coords)) {
                    Controller::model_.play_move(square_coords);
                    //remove any existing text from screen
                    // view_.update_text_box("");

                } else {
                    //handle an invalid move error by printing "invalid move"
                    // view_.update_text_box("Invalid move");
                }

                //now that we've found the closest square, break out
                return;
            }
        }
    }

    //if we got here, mouse click was not associated with any square
    return;
}

//Helper function for on_mouse_down and on_mouse_move. Takes in the
//position of a mouse click and determines whether the position is
//within the current square given by (row,col)
bool Controller::mouse_is_within_square_(ge211::Posn<int> mouse_posn,
                                         int col_ind, int row_ind) {

    //current position we're checking is at (row, col)
    //a mouse click is inside the square given by this position
    //if its position is inside its bounding box
    int square_left = col_ind;
    int square_right = col_ind + 1;
    int square_top = row_ind;
    int square_bottom = row_ind + 1;

    // //for converting between screen position and board index
    double mouse_x_board = mouse_posn.x / double(grid_size);
    double mouse_y_board = mouse_posn.y / double(grid_size);

    //ways the mouse click posn could be outside the bounding
    //box of this square: left of square_left, right of square_right,
    //above square_top, below square_bottom. if none of these are true,
    //mouse click is inside this current box

    return (!(mouse_x_board < square_left || mouse_x_board > square_right
              || mouse_y_board < square_top || mouse_y_board > square_bottom));
}

//when we move the mouse, if we move the mouse over a valid position to play in,
//show the possible flips that will result from that play
void Controller::on_mouse_move(ge211::Posn<int> mouse_posn) {

    //iterate through all the squares of the board
    for (int col_ind = 0; col_ind < Controller::model_.board().width;
         ++col_ind) {
        for (int row_ind = 0; row_ind < Controller::model_.board().height;
             ++row_ind) {

            ge211::Posn<int> square_coords{col_ind,row_ind};

            if (Controller::mouse_is_within_square_(mouse_posn, col_ind,
                                                    row_ind)) {

                //check to see if this move is one of the next moves possible
                // Move const* movep = model_.find_move(square_coords);
                // if (movep) {
                //     //if so, send this position set to view and tell them
                //     //to add sprites to the board for these flips
                //     view_.set_move_preview(movep -> second);
                // } else {
                //     //give an empty posn set so as to clear out the sprites
                //     view_.set_move_preview({{}});
                // }

            }
        }
    }
}

