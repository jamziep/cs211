
#include "game_config.hxx"

//Used specifically for the view. The goal of this file is to
//make one central place where aspects of the view can be changed,
//so as to avoid "magic ints" in the code.
Game_config::Game_config()
        //colors
       :board_color(ge211::Color(97,53,13)),
        dark_color(ge211::Color(217, 171, 128)),
        background_color(ge211::Color(107,16, 49)),
        black_color(ge211::Color(0,0,0)),
        white_color(ge211::Color(255, 255, 255)),
        dark_grey(ge211::Color(145,145,145)),
        light_grey(ge211::Color(200,200,200)),
        bright_red(ge211::Color(190,0,0)),
        parchment(ge211::Color(201,192,141)),

        //locations of text boxes
        black_timer_location{800,125},
        white_timer_location{800,200},
        black_whose_turn_location{800, 500},
        white_whose_turn_location{800, 560},

        //sizes of things
        board_size{1080, 761},
        timer_size{240,75},
        turn_tracker_size{240,50}
{}