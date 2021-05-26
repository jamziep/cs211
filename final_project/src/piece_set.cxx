
#include "piece_set.hxx"

Piece_set::Piece_set(std::initializer_list<value_type> elems)
        : Piece_set(elems.begin(), elems.end())
{ }

// Piece_set
// Piece_set::universe()
// {
//     return ~Piece_set();
// }
//
// bool
// Piece_set::empty() const
// {
//     return bits_.none();
// }
//
// size_t
// Piece_set::size() const
// {
//     return bits_.count();
// }
//
// bool
// Piece_set::operator[](Piece_set::value_type p) const
// {
//     return bits_.test(index_of_checked_(p));
// }
//
// Piece_set::reference
// Piece_set::operator[](Piece_set::value_type p)
// {
//     return bits_[index_of_checked_(p)];
// }
//
// Piece_set&
// Piece_set::operator&=(Piece_set that)
// {
//     bits_ &= that.bits_;
//     return *this;
// }
//
// Piece_set&
// Piece_set::operator|=(Piece_set that)
// {
//     bits_ |= that.bits_;
//     return *this;
// }
//
// Piece_set&
// Piece_set::operator^=(Piece_set that)
// {
//     bits_ ^= that.bits_;
//     return *this;
// }
//
// Piece_set
// Piece_set::operator&(Piece_set that) const
// {
//     Piece_set result(*this);
//     return result &= that;
// }
//
// Piece_set
// Piece_set::operator|(Piece_set that) const
// {
//     Piece_set result(*this);
//     return result |= that;
// }
//
// Piece_set
// Piece_set::operator^(Piece_set that) const
// {
//     Piece_set result(*this);
//     return result ^= that;
// }
//
// Piece_set
// Piece_set::operator~() const
// {
//     Piece_set result(*this);
//     result.bits_ = ~result.bits_;
//     return result;
// }
//
// void
// Piece_set::clear()
// {
//     bits_.reset();
// }
//
// Piece_set::iterator
// Piece_set::begin() const
// {
//     for (size_t i = 0; i < index_limit; ++i) {
//         if (bits_.test(i)) {
//             return iterator_(position_of_(i));
//         }
//     }
//
//     return end();
// }
//
// Piece_set::iterator
// Piece_set::end() const
// {
//     return iterator_(position_of_(index_limit));
// }
//
// bool
// operator==(Piece_set a, Piece_set b)
// {
//     return a.bits_ == b.bits_;
// }
//
// bool
// operator!=(Piece_set a, Piece_set b)
// {
//     return !(a == b);
// }
//
// size_t
// Piece_set::index_of_checked_(value_type p)
// {
//     size_t index = index_of_(p);
//
//     if (index >= index_limit) {
//         throw ge211::Client_logic_error("Piece_set: out of bounds");
//     }
//
//     return index;
// }
//
// size_t
// Piece_set::index_of_(value_type p)
// {
//     return size_t(coord_limit * p.get_posn().x + p.get_posn().y);
// }
//
// Position
// Piece_set::position_of_(size_t index)
// {
//     auto x = int(index / coord_limit);
//     auto y = int(index % coord_limit);
//     return {x, y};
// }
//
// // Piece_set::iterator
// // Piece_set::iterator_(value_type p) const
// // {
// //     return {p, &bits_};
// // }
//
// Piece_set::iterator::iterator(value_type p, bits_t const *bits) noexcept
//         : bits_(bits),
//           current_(p)
// { }
//
// Piece_set::iterator::value_type
// Piece_set::iterator::operator*() const
// {
//     return current_;
// }
//
// Piece_set::iterator::value_type *
// Piece_set::iterator::operator->() const
// {
//     return &current_;
// }
//
// // Piece_set::iterator&
// // Piece_set::iterator::operator++()
// // {
// //     size_t current = index_of_(current_);
// //
// //     do {
// //         current = current == index_limit ? 0 : current + 1;
// //     } while (!stopping_point_(current));
// //
// //     current_ = position_of_(current);
// //
// //     return *this;
// // }
//
// // Piece_set::iterator&
// // Piece_set::iterator::operator--()
// // {
// //     size_t current = index_of_(current_);
// //
// //     do {
// //         current = current == 0 ? index_limit : current - 1;
// //     } while (!stopping_point_(current));
// //
// //     current_ = position_of_(current);
// //
// //     return *this;
// // }
//
// Piece_set::iterator
// Piece_set::iterator::operator++(int)
// {
//     iterator result(*this);
//     ++*this;
//     return result;
// }
//
// Piece_set::iterator
// Piece_set::iterator::operator--(int)
// {
//     iterator result(*this);
//     --*this;
//     return result;
// }
//
// bool
// Piece_set::iterator::stopping_point_(size_t index) const
// {
//     return index == index_limit || bits_->test(index);
// }
//
// // bool
// // operator==(Piece_set::iterator a, Piece_set::iterator b)
// // {
// //     // return *a == *b;
// //
// // }
//
// bool
// operator!=(Piece_set::iterator a, Piece_set::iterator b)
// {
//     return !(a == b);
// }
//
// std::ostream&
// operator<<(std::ostream& os, Piece_set pset)
// {
//     os << "{";
//
//     char const *before_elem = "";
//     for (Piece_set::value_type pos : pset) {
//         os << before_elem << "{" << pos.get_posn().x << ", " << pos.get_posn().y
//         << "}";
//         before_elem = ", ";
//     }
//
//     return os << "}";
// }
