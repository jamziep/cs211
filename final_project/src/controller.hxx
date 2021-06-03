#pragma once

#include "view.hxx"

// The main game class for Reversi.
class Controller : public ge211::Abstract_game
{
public:

    // Constructs a game controller.
    Controller();

    // `protected` means that GE211 (via base class `ge211::Abstract_game`)
    // can access these members, but arbitrary other code cannot:
protected:
    //
    // Controller operations called by GE211
    //

    void on_mouse_down(ge211::Mouse_button, ge211::Posn<int>) override;
    void on_key(ge211::Key) override;
    void on_mouse_move(ge211::Posn<int>) override;

    //specifically for the clock:
    void on_frame(double dt) override;

    // These three delegate to the view:
    void draw(ge211::Sprite_set&) override;
    View::Dimensions initial_window_dimensions() const override;
    std::string initial_window_title() const override;

private:
    Model            model_;
    View             view_;

    //Helper function for on_mouse_down and on_mouse_move. Takes in the
    //position of a mouse click and determines whether the position is
    //within the current square given by (row,col).
    bool mouse_is_within_square_(ge211::Posn<int>, int, int);
};
