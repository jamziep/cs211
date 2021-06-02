#pragma once

#include <ge211.hxx>

struct Game_config {

    /// Board dimensions will use `int` coordinates.
    using Dimensions = ge211::Dims<int>;

    /// Board positions will use `int` coordinates.
    using Position = ge211::Posn<int>;

    /// Board rectangles will use `int` coordinates.
    using Rectangle = ge211::Rect<int>;

    Game_config();

    //colors for things
    ge211::Color board_color;
    ge211::Color dark_color;
    ge211::Color background_color;
    ge211::Color black_color;
    ge211::Color white_color;
    ge211::Color dark_grey;
    ge211::Color light_grey;
    ge211::Color bright_red;
    ge211::Color parchment;

    //locations of text boxes
    Position black_timer_location;
    Position white_timer_location;
    Position black_whose_turn_location;
    Position white_whose_turn_location;

    //sizes of things
    Dimensions board_size;
    Dimensions timer_size;
    Dimensions turn_tracker_size;



};