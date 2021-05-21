#pragma once

#include "model.hxx"

#include <string>

class View
{
public:
    /// View dimensions will use `int` coordinates.
    using Dimensions = ge211::Dims<int>;

    /// View positions will use `int` coordinates.
    using Position = ge211::Posn<int>;

    /// View rectangles will use `int` coordinates.
    using Rectangle = ge211::Rect<int>;

    /// Constructs a view that knows about the given model.
    explicit View(Model const&);

    // TODO: your code here
    // You will probably want to add arguments here so that the
    // controller can communicate UI state (such as a mouse or
    // cursor position):
    void draw(ge211::Sprite_set& set);

    Dimensions initial_window_dimensions() const;

    std::string initial_window_title() const;

    // TODO: Add any public member functions you need.

    //Lets controller update what exists in the text box based on the
    //state of the board.
    void update_text_box(std::string);
    void set_move_preview(Move const*, ge211::Sprite_set&);

    //another version of editing move preview
    void set_move_preview2(Position_set);

    //Lets "view" preview the possible flips caused by a move that the
    //player scrolls over. Takes in a Move map and a reference to a
    //sprite set from View::draw(), and adds any

private:
    Model const& model_;

    // TODO: Add any additional private members you need, such as helper
    // functions or sprites.

public:
    ge211::Circle_sprite const black_tile;
    ge211::Circle_sprite const white_tile;
    ge211::Circle_sprite const gray_tile;
    ge211::Circle_sprite const plays;
    ge211::Rectangle_sprite const board_sprite;
    ge211::Rectangle_sprite const square_sprite;
    ge211::Text_sprite text_sprite; //not const as it changes

    //for if the player mouses over a square to preview a move
    Position_set move_preview;

};
