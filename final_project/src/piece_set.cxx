
#include "piece_set.hxx"

//Constructor
Piece_set::Piece_set(std::vector<Piece> pieces)
    :pieces_(pieces)
{}

//Gets the piece at a given position. Gets a reference
//so you can modify it.

//note: this may be buggy, because I have no idea if you can
//return a piece like this
Piece& Piece_set::get(Position posn) {

    for (Piece& piece : pieces_) {

        //if the position of this piece matches the position
        //passed as input, return a reference to it
        if (piece.get_posn() == posn) {
            return piece;
        }
    }

    //if we didn't find anything, return a piece that doesn't
    //represent anything
    Piece temp_piece(Piece_type::null, Player::neither, {0,0});
    return temp_piece;
}

//finds the size of the piece set by looking at its vector.
size_t Piece_set::size() {
    return pieces_.size();
}

//might not need this, since all the same pieces stay in a
//piece set; we just change their attributes
// //Adds a piece to the piece set.
// void Piece_set::set(Piece apiece) {
//     pieces_.push_back(apiece)
// }

//Removes a piece from the piece set. Accomplished by
//swapping the last thing in the piece set and the current
//thing, then popping the last thing in the piece set b/c
//it is irrelevant.
void Piece_set::remove(Piece apiece) {

    //checks to see if any pieces in the vector match this piece,
    //then sets the piece to inactive
    for (Piece& piece : pieces_) {

        //if we have a match, remove a piece from the board
        //by setting active to false
        if (apiece == piece) {
            piece.remove_piece();
            return;
        }
    }

}


/// Prints a `Piece_set`; suitable for debugging.
// std::ostream&
// operator<<(std::ostream&, Piece_set);

//returns whether a position is found in this piece set
bool Piece_set::operator[](Position posn) const{

    // for (Piece piece : pieces_) {
    // for (int ii = 0; ii < pieces_.size(); ++ii) {
    //     pieces_.at(ii)
    // }

    //idk why none of this is working, so I'll ask for help
}

// Are two piece sets equal?
bool operator==(Piece_set a, Piece_set b){

    //iterate through each piece in the two psets, and see if
    //each piece exists in both psets
    for (int ii = 0; ii < a.size(); ++ii) {
        for (int jj = 0; jj < b.size(); ++jj) {

            //start by taking out a piece from A and comparing it
            //to everything in B. if you find a match, break and
            //go to the next piece to compare
            Piece pieceA = a.pieces_[ii];
            Piece pieceB = b.pieces_[jj];
            if (pieceA == pieceB) {
                break;
            }
        }

        //if we get here, means that one piece in the piece_set a
        //doesn't match anything in B
        return false;
    }

    //else, if we got here, means all the pieces in piece_set a
    //match something in B
    return true;
}

/// Are two piece sets unequal?
bool operator!=(Piece_set a, Piece_set b){

    return !(a == b);
}