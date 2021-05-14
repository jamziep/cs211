#include "controller.hxx"

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

//functions to add to controller:

//on mouse down, try to place a token at the position in board
//that's closest to that mouse click

//if key pressed is q, exit out

//bonus functionality:

//this can be implemented in view as well. show all possible moves
//for the player by showing the outline of a token of that color
//in every square where the player can move. outline should be a
//border only

//on mouse move, show the possible moves for the player. if the
//player hovers over a valid position, make the outline slightly
//darker over that token

