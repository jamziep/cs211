/*******************************************/
/*** DO NOT CHANGE ANYTHING IN THIS FILE ***/
/*******************************************/

#pragma once

#include <stdbool.h>

// Represents a positioned circle.
struct circle
{
    double x, y;
    double radius;
};

// Returns whether a circle is valid, namely, whether its radius is
// positive.
bool valid_circle(struct circle c);

// Reads a circle from stdin. Expects to see x, then y, then the radius.
// If the input ends, returns a circle with a negative (invalid) radius.
struct circle read_circle(void);

// Returns whether the given circles are overlapped. (Tangent circles
// are NOT considered to be overlapped.)
bool overlapped_circles(struct circle, struct circle);
