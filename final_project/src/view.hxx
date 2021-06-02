#pragma once

#include "model.hxx"
#include "game_config.hxx"

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

    //for drawing all sprites:
    void draw(ge211::Sprite_set& set);

    Dimensions initial_window_dimensions() const;

    std::string initial_window_title() const;

    //Lets controller update what exists in the text box based on the
    //state of the board.
    void update_text_box(Player, std::string);

    //Works with controller.cxx. If the user is hovering over a square
    //they can play in, set_move_preview() lets controller add positions
    //which view can create as sprites on the board.
    void set_move_preview(Position_set);

    //Works with controller.cxx. Shows a selected piece with a grey highlight
    // on the piece.
    void set_selected_piece(Position_set);

private:
    Model const& model_;

public:

    //game config for locations, sizes, colors
    Game_config config;

    // board
    ge211::Rectangle_sprite const board_sprite;
    ge211::Rectangle_sprite const dark_squares;
    ge211::Rectangle_sprite const background;
    ge211::Rectangle_sprite const black_matte;
    ge211::Rectangle_sprite const white_matte;
    ge211::Rectangle_sprite const whos_turn;

    // for all of your fun sprites
    ge211::Image_sprite white_pawn;
    ge211::Image_sprite white_rook;
    ge211::Image_sprite white_knight;
    ge211::Image_sprite white_bishop;
    ge211::Image_sprite white_king;
    ge211::Image_sprite white_queen;
    ge211::Image_sprite black_pawn;
    ge211::Image_sprite black_rook;
    ge211::Image_sprite black_knight;
    ge211::Image_sprite black_bishop;
    ge211::Image_sprite black_king;
    ge211::Image_sprite black_queen;

    // for clocks. not const, as the time changes
    ge211::Text_sprite black_time_text;
    ge211::Text_sprite white_time_text;

    // for indicators. Changes when pieces are taken.
    ge211::Text_sprite capture_text;

    // for showing valid moves.
    ge211::Circle_sprite const valid_squares;
    ge211::Circle_sprite const valid_pieces;
    ge211::Circle_sprite const king_check;
    Position_set move_preview;
    Position_set selected_move;

    //helper functions
    void draw_board(ge211::Sprite_set& set);
    void draw_background(ge211::Sprite_set& set);
    void update_capture_text(Piece a, Piece b);
    void show_checkmate(Player p);
};


