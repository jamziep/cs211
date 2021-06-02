#include "view.hxx"
//#include "model.hxx"

// edits: changed Model model_ to Model const&

using namespace ge211;
using Color = ge211::Color;
using Sprite_set = ge211::Sprite_set;

// change this to whatever we need it to be
static int const grid_size = 95;

//default declarations:
static Color board_color = Color(97,53,13);
static Color dark_color = Color(217, 171, 128);
static Color background_color = Color(107,16, 49);
static Color black_color = Color(0,0,0);
static Color white_color = Color(255, 255, 255);
static Color dark_grey = Color(145,145,145);
static Color light_grey = Color(200,200,200);
static Color parchment = Color(201,192,141);


View::View(Model const& model)
        : model_(model),
        // board sprites
          board_sprite({8*grid_size, 8*grid_size}, board_color),
          dark_squares({grid_size, grid_size}, dark_color),
          background({1080, 761}, background_color),
          black_matte({240,75}, black_color),
          white_matte({240,75}, white_color),
          whos_turn({240, 50}, parchment),
          // these are arbitrary values for size and color (for now)

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

          //valid moves:
          valid_pieces(grid_size/2, dark_grey),
          valid_squares(20, light_grey),
          move_preview({{}}),
          selected_move({{}})
// sprite initialization
{}

void View::draw(Sprite_set& set)
{
    //draw the background and the board
    draw_board(set);
    draw_background(set);

    //add the text sprites
    set.add_sprite(black_time_text, {800,125},4);
    set.add_sprite(white_time_text, {800,200},4);

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

        // shows you which pieces can be moved.
        //if (model_.find_move(posn)){
        //    set.add_sprite(valid_pieces,screen_posn, 4);
        //}

        //if the player is previewing a move, take the full list of tiles
        //overturned by that move, and draw all those tiles in gray to preview
    }
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
    set.add_sprite(black_matte, {800,125}, 3);
    set.add_sprite(white_matte, {800,200}, 3);

    // move indicator and piece taken:
    set.add_sprite(whos_turn, {800, 450}, 3);
    set.add_sprite(whos_turn, {800, 510}, 3);
}

View::Dimensions
View::initial_window_dimensions() const
{
    // always return this default window size since we will keep the board
    // and UI constant.
    return Dimensions(1080, 761);
}

std::string
View::initial_window_title() const
{
    // You can change this if you want:
    return "Chess";
}

void View::update_text_box(Player p, std::string text)
{
    //make a new builder for text
    ge211::Font sans30{"sans.ttf", 30};
    ge211::Text_sprite::Builder text_builder(sans30);

    //add the text to our builder and reconfigure
    text_builder.message(text);

    //different colors and sprites for black and white
    if (p == Player::black) {
        text_builder.color(ge211::Color(255,255,255));
        black_time_text.reconfigure(text_builder);
    } else if (p == Player:: white) {
        text_builder.color(ge211::Color(0,0,0));
        white_time_text.reconfigure(text_builder);
    } else {
        throw Client_logic_error("View::update_text_box: can't update"
                                 "the text box of player 'neither'");
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
