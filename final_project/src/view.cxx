#include "view.hxx"

using namespace ge211;
using Sprite_set = ge211::Sprite_set;

static int const grid_size = 95;

// Constructs important different things for the view file. Divided into
// sections as follows: board/background, white sprites, black sprites,
// clocks/monitors, checks and previews.
View::View(Model const& model)
        : model_(model),
          config(),

          board_sprite({8*grid_size, 8*grid_size}, config.board_color),
          dark_squares({grid_size, grid_size}, config.dark_color),
          background(config.board_size, config.background_color),
          black_matte(config.timer_size, config.black_color),
          white_matte(config.timer_size, config.white_color),
          whos_turn(config.turn_tracker_size, config.parchment),

          white_pawn("white_pawn.png"),
          white_rook("white_rook.png"),
          white_knight("white_knight.png"),
          white_bishop("white_bishop.png"),
          white_king("white_king.png"),
          white_queen("white_queen.png"),

          black_pawn("black_pawn.png"),
          black_rook("black_rook.png"),
          black_knight("black_knight.png"),
          black_bishop("black_bishop.png"),
          black_king("black_king.png"),
          black_queen("black_queen.png"),

          black_time_text(),
          white_time_text(),
          capture_text(),
          monitor(),

          valid_squares(20, config.light_grey),
          valid_pieces(grid_size/2, config.dark_grey),
          king_check(grid_size/2, config.bright_red),
          move_preview({{}}),
          selected_move({{}})
{}


// main draw function: Draws the background, board, the white and black
// pieces in the proper place, the clock, the monitor, the capture indicator,
// move previews, and the check indicator.
void View::draw(Sprite_set& set)
{
    draw_board(set);
    draw_background(set);

    set.add_sprite(black_time_text, config.black_timer_location,4);
    set.add_sprite(white_time_text, config.white_timer_location,4);
    if(!capture_text.empty()) {
        set.add_sprite(capture_text, config.capture_text_location, 4);
    }
    monitor_update(set);

    for (Position posn : View::model_.board()) {
        auto curr_piece = View::model_[posn];

        Position screen_posn = {posn.x * grid_size, posn.y * grid_size};

        if (curr_piece.get_player() == Player::black) {
            switch (curr_piece.get_piece_type()) {
            case Piece_type::pawn:
                set.add_sprite(black_pawn, screen_posn, 5);
                break;
            case Piece_type::rook:
                set.add_sprite(black_rook, screen_posn, 5);
                break;
            case Piece_type::knight:
                set.add_sprite(black_knight, screen_posn, 5);
                break;
            case Piece_type::bishop:
                set.add_sprite(black_bishop, screen_posn, 5);
                break;
            case Piece_type::queen:
                set.add_sprite(black_queen, screen_posn, 5);
                break;
            case Piece_type::king:
                set.add_sprite(black_king, screen_posn, 5);
                break;
            case Piece_type::null: // skip it
                break;
            }


        } else if (curr_piece.get_player() == Player::white) {
            switch (curr_piece.get_piece_type()) {
            case Piece_type::pawn:
                set.add_sprite(white_pawn, screen_posn, 5);
                break;
            case Piece_type::rook:
                set.add_sprite(white_rook, screen_posn, 5);
                break;
            case Piece_type::knight:
                set.add_sprite(white_knight, screen_posn, 5);
                break;
            case Piece_type::bishop:
                set.add_sprite(white_bishop, screen_posn, 5);
                break;
            case Piece_type::queen:
                set.add_sprite(white_queen, screen_posn, 5);
                break;
            case Piece_type::king:
                set.add_sprite(white_king, screen_posn, 5);
                break;
            case Piece_type::null:
                break;
            }
        } else {
        }
    }

    if (!move_preview.empty()){
        for (Position posn : move_preview) {
            Position screen_posn{posn.x * grid_size + 28, posn.y*grid_size +
            28};
            set.add_sprite(valid_squares,screen_posn,4);
        }
    }

    if (!selected_move.empty()){
        for (Position posn : selected_move){
            Position screen_posn{posn.x * grid_size, posn.y*grid_size};
            set.add_sprite(valid_pieces, screen_posn, 4);
        }
    }


    if (model_.is_in_check(Player::black)){
        Position king_posn = model_.find_king(Player::black);
        Position screen_posn{king_posn.x * grid_size, king_posn.y * grid_size};
        set.add_sprite(king_check, screen_posn, 4);

    } else if (model_.is_in_check(Player::white)) {
        Position king_posn = model_.find_king(Player::white);
        Position screen_posn{king_posn.x * grid_size, king_posn.y * grid_size};
        set.add_sprite(king_check, screen_posn, 4);
    }

}


// HELPERS //

// helper for Draw. Draws the background board and the alternating colors.
void View::draw_board(Sprite_set& set)
{
    set.add_sprite(View::board_sprite, ge211::Posn<int>{0,0} );

    for (Position posn : View::model_.board()) {
        if (posn.y % 2 == 0) {
            if (posn.x % 2 != 0) {
                set.add_sprite(View::dark_squares, {posn.x*grid_size, posn
                .y*grid_size}, 2);
            }
        } else {
            if (posn.x % 2 == 0) {
                set.add_sprite(View::dark_squares, {posn.x*grid_size, posn
                .y*grid_size}, 2);
            }
        }
    }
}

// helper for drawing the background, the clock, and the capture/turn indicator.
void View::draw_background(Sprite_set& set)
{
    set.add_sprite(background, {0,0}, 0);

    set.add_sprite(black_matte, config.black_timer_location, 3);
    set.add_sprite(white_matte, config.white_timer_location, 3);

    set.add_sprite(whos_turn, config.capture_location, 3);
    set.add_sprite(whos_turn, config.monitor_location, 3);
}

View::Dimensions
// Always returns the same size since board will always be 8x8.
View::initial_window_dimensions() const
{
    return config.board_size;
}

std::string
View::initial_window_title() const
{
    return "Chess";
}

// Helper for the clocks. Builds the clock time and converts the total amount
// of seconds into standard time (i.e 0:00)
void View::update_time_text(Player p, std::string text)
{
    ge211::Font sans30{"open_sans.ttf", 30};
    ge211::Text_sprite::Builder text_builder(sans30);

    text_builder.message(text);

    if (p == Player::black) {
        text_builder.color(config.white_color);
        black_time_text.reconfigure(text_builder);
    } else if (p == Player:: white) {
        text_builder.color(config.black_color);
        white_time_text.reconfigure(text_builder);
    } else {
        throw Client_logic_error("View::update_time_text: can't update"
                                 "the text box of player 'neither'");
    }
}

//let piece A be the piece that's capturing and B is the piece
//that's being captured. This function will update the appropriate
//text box
void View::update_capture_text(Piece a, Piece b)
{
    ge211::Font sans20{"sans.ttf", 17};
    ge211::Text_sprite::Builder text_builder(sans20);
    std::string atype = " ";
    std::string btype = " ";
    std::string takes = "takes ";
    std::string aplayer = " ";
    std::string bplayer = " ";

    if (a.get_player() == Player::white){
        aplayer = "White ";
        bplayer = "black ";
    } else if(a.get_player() == Player::black) {
        aplayer = "Black ";
        bplayer = "white ";
    }

    switch(a.get_piece_type()) {
        case Piece_type::pawn: {
           atype = "pawn ";
           break;
        }
        case Piece_type::rook: {
           atype = "rook ";
           break;
        }
        case Piece_type::knight: {
            atype = "knight ";
            break;
        }
        case Piece_type::bishop: {
            atype = "bishop ";
            break;
        }
        case Piece_type::queen: {
            atype = "queen ";
            break;
        }
        case Piece_type::king: {
            atype = "king ";
            break;
        }
        default: {
            atype = "null?";
            break;
        }
    }

    switch(b.get_piece_type()) {
        case Piece_type::pawn: {
            btype = "pawn.";
            break;
        }
        case Piece_type::rook: {
            btype = "rook.";
            break;
        }
        case Piece_type::knight: {
            btype = "knight.";
            break;
        }
        case Piece_type::bishop: {
            btype = "bishop.";
            break;
        }
        case Piece_type::queen: {
            btype = "queen.";
            break;
        }
        default:{
            btype = "null.";
            break;
        }
    }
    takes = aplayer + atype + takes + bplayer + btype;
    text_builder.message(takes);
    text_builder.color(config.capture_color);
    capture_text.reconfigure(text_builder);
}

// updates the monitor on the sidebar of the chessboard. This monitor will
// display important information, like who's turn it is and if a checkmate
// has been served. Also checks to see if the clocks have run out of time and
// prints out winner on time.
void View::monitor_update(Sprite_set& set)
{
    ge211::Font sans20{"sans.ttf", 17};
    ge211::Text_sprite::Builder text_builder(sans20);
    std::string monitors = " ";
    if (model_.turn() == Player::white){
        monitors = "It is white's turn.";
    } else if (model_.turn() == Player::black){
        monitors = "It is black's turn.";
    } else if (model_.winner() == Player::white){

        if (model_.turn() == Player::out_of_time) {
            monitors = "Out of time! White wins.";
        } else {
            monitors = "Checkmate. White wins.";
        }

    }else if (model_.winner() == Player::black){

        if (model_.turn() == Player::out_of_time) {
            monitors = "Out of time! Black wins.";
        } else {
            monitors = "Checkmate. Black wins.";
        }

    } else if (model_.winner() == Player::neither) {
        monitors = "Stalemate.";
    }
    text_builder.message(monitors);
    text_builder.color(config.capture_color);
    monitor.reconfigure(text_builder);
    set.add_sprite(monitor,config.monitor_text_location, 6);

}

void View::set_move_preview(Position_set pset)
{
    View::move_preview = pset;
}

void View::set_selected_piece(Position_set pset)
{
    View::selected_move = pset;
}
