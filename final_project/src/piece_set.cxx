
#include "piece_set.hxx"

//Constructor
Piece_set::Piece_set(std::vector<Piece> pieces)
    :pieces_(pieces)
{}

//Gets the piece at a given position. Gets a reference
//so you can modify it.

//note: this may be buggy, because I have no idea if you can
//return temp_piece like this
// Piece& Piece_set::get(Position posn) {
//
//     for (Piece& piece : pieces_) {
//
//         //if the position of this piece matches the position
//         //passed as input, return a reference to it
//         if (piece.get_posn() == posn && piece.is_active()) {
//             return piece;
//         }
//     }
//
//     //if we didn't find anything, return a piece that doesn't
//     //represent anything
//     Piece temp_piece(Piece_type::null, Player::neither, {0,0});
//     return temp_piece;
// }

// Piece* Piece_set::get_piece_ptr(Position posn)
// {
//     //I really want to use a reference for this so that
//     //we can modify the member data within pieces_ with this
//     //pointer
//
//     // for (Piece& piece : pieces_) {
//     for (Piece& piece : pieces_) {
//
//         //if the position of this piece matches the position
//         //passed as input, return a reference to it
//         if (piece.get_posn() == posn && piece.is_active()) {
//             return &piece;
//         }
//     }
//
//     //if we didn't find anything, return a piece that doesn't
//     //represent anything
//     return nullptr;
// }

Piece Piece_set::get_piece_from_set(Position posn) const
{
    //I really want to use a reference for this so that
    //we can modify the member data within pieces_ with this
    //pointer

    // for (Piece& piece : pieces_) {
    for (Piece piece : pieces_) {

        //if the position of this piece matches the position
        //passed as input, return a reference to it
        if (piece.get_posn() == posn && piece.is_active()) {
            return piece;
        }
    }

    //if we didn't find anything, return a piece that doesn't
    //represent anything
    return Piece(Piece_type::null, Player::black, Position {0,0});
}


//finds the size of the piece set by looking at its vector.
size_t Piece_set::size() {
    return pieces_.size();
}

//takes in a piece and a position, and modifies the position
//of that existing piece by reference
// void Piece_set::set_posn(Piece* piece_ptr, Position posn) {
//
//     //maybe do bounds checking on this?
//
//     //iterate through the piece set and see if we have a match
//     //between the desired piece and a piece in the set. if so,
//     //change the position of that piece
//     for (Piece& piece : pieces_) {
//
//         if (piece == *piece_ptr) {
//             piece.set_posn(posn);
//             return;
//         }
//     }
//
// }

void Piece_set::set_posn(Piece apiece, Position posn) {

    //maybe do bounds checking on this?

    //iterate through the piece set and see if we have a match
    //between the desired piece and a piece in the set. if so,
    //change the position of that piece
    for (Piece& piece : pieces_) {

        if (piece == apiece) {
            piece.set_posn(posn);
            return;
        }
    }

}


//takes in a position, gets the piece that corresponds to that
//position on the board, and modifies the position of the piece
//to the new position
void Piece_set::change_posn(Position start, Position end) {

    //get a pointer to piece
    // Piece& piece = get(start);
    Piece piece = get_piece_from_set(start);

    set_posn(piece, end);
}

//"Removes" a piece from the piece set by setting the
//piece to inactive.
void Piece_set::remove(Piece apiece) {

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

    for (Piece piece : pieces_) {
        if (piece.get_posn() == posn) {
            return true;
        }
    }
    //else, if we didn't find a piece that matches this
    //posn, return false--the position does not appear to be
    //occupied
    return false;

    //fixed, I think: I removed the copy constructor from piece.hxx
}

//for iterating through a piece_set
Piece Piece_set::operator[](size_t ii) {

    return pieces_[ii];
}


// Are two piece sets equal?
bool operator==(Piece_set a, Piece_set b){

    //iterate through each piece in the two sets, and see if
    //each piece exists in both sets
    for (size_t ii = 0; ii < a.size(); ++ii) {
        for (size_t jj = 0; jj < b.size(); ++jj) {

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

// Are two piece sets unequal?
bool operator!=(Piece_set a, Piece_set b){

    return !(a == b);
}


