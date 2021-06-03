#pragma once

#include <ge211.hxx>

struct Game_config {

    /// Board dimensions will use `int` coordinates.
    using Dimensions = ge211::Dims<int>;

    /// Board positions will use `int` coordinates.
    using Position = ge211::Posn<int>;

    /// Board rectangles will use `int` coordinates.
    using Rectangle = ge211::Rect<int>;

    ///Constructor for all values.
    Game_config();

    ///Converts a double value of seconds into printable text.
    ///Used to print out the time to the board in view
    std::string seconds_to_text(double);


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
    ge211::Color capture_color;

    //locations of text boxes
    Position black_timer_location;
    Position white_timer_location;
    Position capture_location;
    Position monitor_location;
    Position capture_text_location;
    Position monitor_text_location;

    //sizes of things
    Dimensions board_size;
    Dimensions timer_size;
    Dimensions turn_tracker_size;

    //max time on timer, in seconds
    int time_limit = 600;
};