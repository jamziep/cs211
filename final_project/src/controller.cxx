#include "controller.hxx"

static int const grid_size = 95;

//copied this value from view.cxx for finding size of board
//ideally I'd like to include the entire view.cxx file, since
//these controls depend on the visual board dims

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

//on mouse down, select a piece. switch the select bool. If the same tile is
// clicked again, then we return the select bool back to false. If we click
// on another valid square, then we move the piece to that square.

bool selected = false;
ge211::Posn<int> selected_posn{0, 0};
void Controller::on_mouse_down(ge211::Mouse_button btn,
                               ge211::Posn<int> mouse_posn)
{
    for (int col_ind = 0; col_ind < 8; ++col_ind) {
        for (int row_ind = 0; row_ind < 8; ++row_ind) {
            if (Controller::mouse_is_within_square_(mouse_posn, col_ind,
                                                    row_ind)) {
                // if selected is false, we switch selected to true and break
                // out.
                // gives you selected coordinates
                ge211::Posn<int> square_coords{col_ind, row_ind};



                if (!selected &&
                Controller::model_.find_move(square_coords)) {
                    selected = true;
                    selected_posn = square_coords;
                    return;
                    // deselect by clicking any square that is invalid. (Not
                    // implemented yet)


                    // otherwise check for valid move. if valid, play move to
                    // the newly selected square.
                } else {
                    model_.play_move(selected_posn, square_coords);
                    std::cout << "valid move found" << "\n"; // DEBUG
                    selected = false;
                    return;
                }
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
                // check to see if this move is possible
                Move const* movep = model_.find_move(square_coords);
                if (movep) {
                    //if so, send this position set to view and tell them
                    //to add sprites to the board for these flips
                    view_.set_move_preview(movep -> second);
                } else {
                    //give an empty posn set so as to clear out the sprites
                    view_.set_move_preview({{}});
                }
            }
        }
    }
}


void Controller::on_frame(double dt) {

    //update the time shown on the board for black and white. this may
    //eventually be better suited for an on_frame() function

    //cast doubles to ints b/c that's all we need to show
    double black_total_seconds = model_.elapsed_time_black().seconds();
    double white_total_seconds = model_.elapsed_time_white().seconds();

    int black_minutes = (int)black_total_seconds/60;
    int white_minutes = (int)white_total_seconds/60;
    int black_seconds = black_total_seconds;
    int white_seconds = white_total_seconds;

    //turn these times into strings
    std::string black_text = "";
    if (black_minutes < 10) {
        black_text += "0";
    }
    black_text += std::to_string(black_minutes);
    black_text += ":";
    if (black_seconds < 10) {
        black_text += "0";
    }
    black_text += std::to_string(black_seconds);

    std::string white_text = "";
    if (white_minutes < 10) {
        white_text += "0";
    }
    white_text += std::to_string(white_minutes);
    white_text += ":";
    if (white_seconds < 10) {
        white_text += "0";
    }
    white_text += std::to_string(white_seconds);

    //update the text sprites
    view_.update_text_box(Player::black, black_text);
    view_.update_text_box(Player::white, white_text);


}
