#include "controller.hxx"

static int const grid_size = 95;

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
// on another valid square, then we move the piece to that square. This
// function also triggers a lot of the important sprites, such as the capture
// indicator and the game monitor.
bool selected = false;
ge211::Posn<int> selected_posn{0, 0};
Position_set starts {};

void Controller::on_mouse_down(ge211::Mouse_button btn,
                               ge211::Posn<int> mouse_posn)
{
    for (int col_ind = 0; col_ind < 8; ++col_ind) {
        for (int row_ind = 0; row_ind < 8; ++row_ind) {
            if (Controller::mouse_is_within_square_(mouse_posn, col_ind,
                                                    row_ind)) {
                ge211::Posn<int> square_coords{col_ind, row_ind};

                if (!selected &&
                Controller::model_.find_move(square_coords)) {
                    selected = true;
                    selected_posn = square_coords;
                    starts[selected_posn] = true;
                    view_.set_selected_piece(starts);
                    return;

                } else if(selected) {

                    Move const* movep = model_.find_move(selected_posn);
                    Position_set possible_moves = movep -> second;

                    if (possible_moves[square_coords]) {
                        auto selected_piece = model_[selected_posn];
                        auto to_piece = model_[square_coords];

                        if(to_piece.get_piece_type() != Piece_type::null) {
                            view_.update_capture_text(selected_piece, to_piece);
                        }
                        model_.play_move(selected_posn, square_coords);
                    }

                    selected = false;
                    starts.clear();
                    view_.set_selected_piece(starts);

                    return;
                }
            }
        }
    }

    return;
}

//Ways a mouse posn could be outside the bounding
//box of this square: left of square_left, right of square_right,
//above square_top, below square_bottom. If none of these are true,
//mouse is inside this current box.
bool Controller::mouse_is_within_square_(ge211::Posn<int> mouse_posn,
                                         int col_ind, int row_ind) {

    int square_left = col_ind;
    int square_right = col_ind + 1;
    int square_top = row_ind;
    int square_bottom = row_ind + 1;

    double mouse_x_board = mouse_posn.x / double(grid_size);
    double mouse_y_board = mouse_posn.y / double(grid_size);


    return (!(mouse_x_board < square_left || mouse_x_board > square_right
              || mouse_y_board < square_top || mouse_y_board > square_bottom));
}

//When we move the mouse, if we move the mouse over a valid position to play in,
//show the possible moves that correspond to the piece in that position via
// grey dots. When we move the mouse away, the grey dots should disappear or
// change to whatever player the cursor is hovering over.
void Controller::on_mouse_move(ge211::Posn<int> mouse_posn) {

    for (int col_ind = 0; col_ind < Controller::model_.board().width;
         ++col_ind) {
        for (int row_ind = 0; row_ind < Controller::model_.board().height;
             ++row_ind) {

            ge211::Posn<int> square_coords{col_ind,row_ind};

            if (Controller::mouse_is_within_square_(mouse_posn, col_ind,
                                                    row_ind)) {

                Move const* movep = model_.find_move(square_coords);
                if (movep) {
                    view_.set_move_preview(movep -> second);
                } else {
                    view_.set_move_preview({{}});
                }
            }
        }
    }
}

//Updates the time shown on the board for black and white.
//Accesses the time from the timer, then turns it into a string.
void Controller::on_frame(double dt) {

    double black_total_seconds = model_.elapsed_time_black().seconds();
    double white_total_seconds = model_.elapsed_time_white().seconds();

    std::string black_text = view_.config.seconds_to_text(black_total_seconds);
    std::string white_text = view_.config.seconds_to_text(white_total_seconds);

    view_.update_time_text(Player::black, black_text);
    view_.update_time_text(Player::white, white_text);

    if (model_.elapsed_time_black().seconds() > view_.config.time_limit
        || model_.elapsed_time_white().seconds() > view_.config.time_limit) {
        model_.set_out_of_time(view_.config.time_limit);
    }
}

