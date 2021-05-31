#include "view.hxx"

using namespace ge211;
using Color = ge211::Color;
using Sprite_set = ge211::Sprite_set;

// change this to whatever we need it to be
static int const grid_size = 90;

View::View(Model const& model)
        : model_(model),
        // board sprites
          board_sprite({8*grid_size, 8*grid_size}, Color(198,161,126)),
          dark_squares({grid_size, grid_size}, Color(10,10,10)),
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
          black_queen("black_queen.png")

// sprite initialization
{}

// DRAW
// pretend that the big draw function is written here
// DRAW
void View::draw(Sprite_set& set) {

    //first, let's draw the board
    draw_board(set);
}



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
    return "Chess";
}

