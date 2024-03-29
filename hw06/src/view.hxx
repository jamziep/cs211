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

    // You will probably want to add arguments here so that the
    // controller can communicate UI state (such as a mouse or
    // cursor position):
    void draw(ge211::Sprite_set& set);

    Dimensions initial_window_dimensions() const;

    std::string initial_window_title() const;

    //Lets controller update what exists in the text box based on the
    //state of the board.
    void update_text_box(std::string);

    //Works with controller.cxx. If the user is hovering over a square
    //they can play in, set_move_preview() lets controller add positions
    //which view can create as sprites on the board.
    void set_move_preview(Position_set);

private:
    Model const& model_;

public:
    ge211::Circle_sprite const black_tile;
    ge211::Circle_sprite const white_tile;
    ge211::Circle_sprite const gray_tile;
    ge211::Rectangle_sprite const board_sprite;
    ge211::Rectangle_sprite const square_sprite;
    ge211::Rectangle_sprite const tile_sprite;
    ge211::Text_sprite text_sprite; //not const as it changes

    //for if the player scrolls over a square to preview a move
    Position_set move_preview;

};
