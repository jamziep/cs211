#include "view.hxx"

using namespace ge211;
using Sprite_set = ge211::Sprite_set;

static int const grid_size = 95;

View::View(Model const& model)
        : model_(model),
          config(),

          // board sprites
          board_sprite({8*grid_size, 8*grid_size}, config.board_color),
          dark_squares({grid_size, grid_size}, config.dark_color),
          background(config.board_size, config.background_color),
          black_matte(config.timer_size, config.black_color),
          white_matte(config.timer_size, config.white_color),
          whos_turn(config.turn_tracker_size, config.parchment),

          //white sprites:
          white_pawn("white_pawn.png"),
          white_rook("white_rook.png"),
          white_knight("white_knight.png"),
          white_bishop("white_bishop.png"),
          white_king("white_king.png"),
          white_queen("white_queen.png"),

          //black sprites:
          black_pawn("black_pawn.png"),
          black_rook("black_rook.png"),
          black_knight("black_knight.png"),
          black_bishop("black_bishop.png"),
          black_king("black_king.png"),
          black_queen("black_queen.png"),

          //clock sprites
          black_time_text(),
          white_time_text(),

          // indicators
          black_capture_text(),
          white_capture_text(),

          //valid moves:
          valid_squares(20, config.light_grey),
          valid_pieces(grid_size/2, config.dark_grey),
          king_check(grid_size/2, config.bright_red),
          move_preview({{}}),
          selected_move({{}})
{}

void View::draw(Sprite_set& set)
{
    //draw the background and the board
    draw_board(set);
    draw_background(set);

    //add the text sprites
    set.add_sprite(black_time_text, config.black_timer_location,4);
    set.add_sprite(white_time_text, config.white_timer_location,4);

    // next, iterate through all the squares and draw each piece.
    for (Position posn : View::model_.board()) {
        auto curr_piece = View::model_[posn];

        Position screen_posn = {posn.x * grid_size, posn.y * grid_size};

        // draws black pieces
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

            // draws white pieces
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
            case Piece_type::null: // skip it
                break;
            }
        } else {
            //like the "default" case
        }
    }

    //if the player is previewing a move, take the full list of tiles
    //overturned by that move, and draw all those tiles in gray to preview
    if (!move_preview.empty()){
        for (Position posn : move_preview) {
            Position screen_posn{posn.x * grid_size + 28, posn.y*grid_size +
            28};
            set.add_sprite(valid_squares,screen_posn,4);
        }
    }
    // if the player has selected a move, draw a gray square over the piece.
    if (!selected_move.empty()){
        for (Position posn : selected_move){
            Position screen_posn{posn.x * grid_size, posn.y*grid_size};
            set.add_sprite(valid_pieces, screen_posn, 4);
        }
    }

    //if either of the kings are in check, draw a red tile behind them
    if (model_.is_in_check(Player::black, true)){
        Position king_posn = model_.find_king(Player::black);
        Position screen_posn{king_posn.x * grid_size, king_posn.y * grid_size};
        set.add_sprite(king_check, screen_posn, 4);

    } else if (model_.is_in_check(Player::white, true)) {
        Position king_posn = model_.find_king(Player::white);
        Position screen_posn{king_posn.x * grid_size, king_posn.y * grid_size};
        set.add_sprite(king_check, screen_posn, 4);
    }

    //TODO: visually show game over

    if (model_.turn() == Player::neither) {
        if (model_.winner() == Player::black) {
            update_capture_text2(Player::black, "Checkmate. Black wins");
            update_capture_text2(Player::white, "");
        } else if (model_.winner() == Player::white) {
            update_capture_text2(Player::white, "Checkmate. White wins");
            update_capture_text2(Player::black, "");
        }
    } else {
        update_capture_text2(Player::black, "");
        update_capture_text2(Player::white, "");
    }

    if (black_capture_text) {
        set.add_sprite(black_capture_text, config.black_whose_turn_location, 6);
    }

    if (white_capture_text) {
        set.add_sprite(white_capture_text, config.white_whose_turn_location, 6);
    }

}


// HELPERS //

// helper for Draw. Draws the background board and the alternating colors.
void View::draw_board(Sprite_set& set)
{
    //draw the board sprite, a rectangle located at 0,0
    set.add_sprite(View::board_sprite, ge211::Posn<int>{0,0} );

    // for putting down a checkerboard
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

// helper for drawing the background and the clock.
void View::draw_background(Sprite_set& set)
{
    // background:
    set.add_sprite(background, {0,0}, 0);

    // clock:
    set.add_sprite(black_matte, config.black_timer_location, 3);
    set.add_sprite(white_matte, config.white_timer_location, 3);

    // capture and turn indicator:
    set.add_sprite(whos_turn, config.black_whose_turn_location, 3);
    set.add_sprite(whos_turn, config.white_whose_turn_location, 3);
}

View::Dimensions
View::initial_window_dimensions() const
{
    // always return this default window size since we will keep the board
    // and UI constant.
    return config.board_size;
}

std::string
View::initial_window_title() const
{
    // You can change this if you want:
    return "Chess";
}

void View::update_time_text(Player p, std::string text)
{
    //make a new builder for text
    ge211::Font sans30{"open_sans.ttf", 30};
    ge211::Text_sprite::Builder text_builder(sans30);

    //add the text to our builder and reconfigure
    text_builder.message(text);

    //different colors and sprites for black and white
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
    ge211::Font sans30{"open_sans.ttf", 30};
    ge211::Text_sprite::Builder text_builder(sans30);

}

void View::update_capture_text2(Player p, std::string text)
{
    ge211::Font sans_small{"open_sans.ttf", 21};
    ge211::Text_sprite::Builder text_builder(sans_small);
    text_builder.color(config.black_color);
    text_builder.message(text);

    switch(p) {
        case Player::white: {
            white_capture_text.reconfigure(text_builder);
            break;
        }
        case Player::black: {
            black_capture_text.reconfigure(text_builder);
            break;
        }
        default: {
            throw Client_logic_error("View:: can't update capture"
                                     " text for non-b/w player");
        }
    }
}

void View::set_move_preview(Position_set pset)
{
    View::move_preview = pset;
}

void View::set_selected_piece(Position_set pset)
{
    View::selected_move = pset;
}
