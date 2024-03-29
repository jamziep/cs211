#pragma once

#include "position_set.hxx"

#include <unordered_map>
#include <utility>

/// A move. The two fields are:
///
///   - first:  the current position of a piece
///
///   - second: all positions where that piece could move, NOT including "first"
///
/// See the documentation for std::pair for details.
using Move = std::pair<ge211::Posn<int> const, Position_set>;

/// A map from move positions to their change sets.  For some key type `K` and
/// value type `V`, a `std::unordered_map<K, V>` acts like a container holding
/// `std::pair<K, V>`s in which 1) the `K` values are distinct, and 2) pairs
/// can be found easily by their `K` values. Thus a `Move_map` is a collection
/// of `Move`s that we can look up by `first` (initial position).
using Move_map = std::unordered_map<ge211::Posn<int>, Position_set>;

/// Prints a `Move`; suitable for debugging.
std::ostream&
operator<<(std::ostream&, Move const&);
