#include "view.hxx"

// Convenient type aliases:
using Color = ge211::Color;
using Sprite_set = ge211::Sprite_set;

// You can change this or even determine it some other way:
static int const grid_size = 36;

View::View(Model const& model)
        : model_(model),
          black_tile(20, Color(0, 0, 0)),
          white_tile(20, Color(255, 255, 255)),
          //empty_square({20, 20}, Color(61,165,198))
          board({20,20}, Color(61,165,198))

        // You may want to add sprite initialization here
{ }

void View::draw(Sprite_set& set)
{
    // TODO, PROBABLY
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