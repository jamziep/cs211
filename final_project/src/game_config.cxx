
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
        capture_color(ge211::Color(10,10,10)),

        //locations of text boxes
        black_timer_location{800,125},
        white_timer_location{800,200},
        capture_location{800, 500},
        monitor_location{800, 560},
        capture_text_location{805, 510},
        monitor_text_location{805, 570},

        //sizes of things
        board_size{1080, 761},
        timer_size{240,75},
        turn_tracker_size{240,50}
{}

//A helper for turning the time of either of the timers from
//a double (total seconds) to a legible string.
std::string Game_config::seconds_to_text(double total_seconds)
{
    int minutes = (int)total_seconds / 60;
    int seconds = (int)total_seconds % 60;

    std::string text = "";
    if (minutes < 10) {
        text += "0";
    }
    text += std::to_string(minutes);
    text += ":";
    if (seconds < 10) {
        text += "0";
    }
    text += std::to_string(seconds);
    return text;
}