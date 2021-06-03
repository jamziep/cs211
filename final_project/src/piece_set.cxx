
#include "piece_set.hxx"

//Constructor
Piece_set::Piece_set(std::vector<Piece> pieces)
    :pieces_(pieces)
{}

//If the position of any piece in the piece_set matches the position
//passed as input, returns a copy of that piece.
//Else, returns a piece that doesn't represent anything.
Piece Piece_set::get_piece_from_set(Position posn) const
{
    for (Piece piece : pieces_) {
        if (piece.get_posn() == posn && piece.is_active()) {
            return piece;
        }
    }

    return Piece(Piece_type::null, Player::black, Position {0,0});
}

//finds the size of the piece set by looking at its vector.
size_t Piece_set::size() {
    return pieces_.size();
}

//Takes in a position, gets the piece that corresponds to that
//position on the board, and modifies the position of the piece
//to the new position.
void Piece_set::change_posn(Position start, Position end) {

    Piece apiece = get_piece_from_set(start);
    for (Piece& piece : pieces_) {

        if (piece == apiece) {
            piece.set_posn(end);
            return;
        }
    }
}

//"Removes" a piece from the piece set by setting the
//piece to inactive.
void Piece_set::remove(Piece apiece) {

    for (Piece& piece : pieces_) {
        if (apiece == piece) {
            piece.remove_piece();
            return;
        }
    }
}

//Returns whether a position is found in this piece set.
bool Piece_set::operator[](Position posn) const{

    for (Piece piece : pieces_) {
        if (piece.get_posn() == posn && piece.is_active()) {
            return true;
        }
    }

    return false;
}

//for iterating through a piece_set
Piece Piece_set::operator[](size_t ii) {

    return pieces_[ii];
}


//Iterate through each piece in the two sets, and see if
//each piece exists in both sets.
//Start by taking out a piece from A and comparing it
//to everything in B. If you find a match, break and
//go to the next piece to compare
bool operator==(Piece_set a, Piece_set b){

    for (size_t ii = 0; ii < a.size(); ++ii) {
        for (size_t jj = 0; jj < b.size(); ++jj) {

            Piece pieceA = a.pieces_[ii];
            Piece pieceB = b.pieces_[jj];
            if (pieceA == pieceB) {
                break;
            }
        }

        //else: a doesn't match anything in b
        return false;
    }

    //all the pieces in piece_set a match something in B
    return true;
}

// Are two piece sets unequal?
bool operator!=(Piece_set a, Piece_set b){

    return !(a == b);
}

// Change a piece in a piece_set at a given position to a different type.
void Piece_set::change_piece_as(Position posn)
{
    Piece apiece = get_piece_from_set(posn);
    for (Piece& piece : pieces_) {

        if (piece == apiece) {
            piece.promote_piece();
            return;
        }
    }
}


