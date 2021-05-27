//
// Created by seanp on 5/25/2021.
//
#pragma once

#include "model.hxx"

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

    void draw_board(ge211:Sprite_set& set);

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
    // for all of your fun sprites
    ge211::Image_sprite const white_pawn;
    ge211::Image_sprite const white_rook;
    ge211::Image_sprite const white_knight;
    ge211::Image_sprite const white_bishop;
    ge211::Image_sprite const white_king;
    ge211::Image_sprite const white_queen;
    ge211::Image_sprite const black_pawn;
    ge211::Image_sprite const black_rook;
    ge211::Image_sprite const black_knight;
    ge211::Image_sprite const black_bishop;
    ge211::Image_sprite const black_king;
    ge211::Image_sprite const black_queen;

    // board shit
    ge211::Rectangle_sprite const board_sprite;
    ge211::Rectangle_sprite const dark_squares;
};

