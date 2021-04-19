// test_circle - Tests for the functions in ../src/circle.c.

#include "../src/circle.h"
#include <211.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

// "Factory function" for easily constructing circles.
struct circle C(double x, double y, double radius)
{
    return (struct circle) {x, y, radius};
}

// Tests that tangent circles are not considered overlapped.
void test_tangent(void)
{
    // Hint: “!” means “not”, so this is an assertion that the
    // two circles *don’t* overlap.
    CHECK(! overlapped_circles(C(0, 0, 2), C(4, 0, 2)));
}

// Tests that more distance circles are not considered overlapped.
void test_not_overlapped(void)
{
    CHECK(! overlapped_circles(C(0, 0, 2), C(5, 0, 2)));
    CHECK(! overlapped_circles(C(0, 0, 2), C(6, 0, 2)));
}

//test_overlapped_task adds two additional CHECKs for overlapped_circles
void test_overlapped_task(void) {

    //test: different but overlapping circles
    CHECK( overlapped_circles( C(0,0,2), C(2,4,18) ) );

    //test: identical circles
    CHECK( overlapped_circles( C(-2,-4,6), C(-2,-4,6) ) ); 
}

int main(void)
{
    test_tangent();
    test_not_overlapped();

    //my test function
    test_overlapped_task();
}
