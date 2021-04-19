#include "circle.h"
#include <stdio.h>

//valid_circle takes in an instance of struct circle and
//checks that it's a circle with radius > 0
bool valid_circle(struct circle c)
{
    //find out data about the radius of circle
    double circle_radius = c.radius;

    //if radius is positive, it's a valid circle
    return circle_radius > 0;
}


//read_circle creates 3 inputs from standard input and makes an
//instance of struct circle
struct circle read_circle(void)
{
    //basic goal of reading inputs
    double x, y, radius;

    //use %g directive for doubles !EDITED: lg is what compiler wants
    int valid_inputs = scanf("%lg%lg%lg", &x, &y, &radius);

    //check to see if all the inputs to scanf were valid
    if (valid_inputs == 3) {
        struct circle c1 = {x, y, radius};
        return c1;
        
    } else {
        struct circle c1 = {0.0, 0.0, -1.0};
        return c1;
    }
}


//overlapped_circles checks the distance and radii of two circles
//to see if their areas intersect
bool overlapped_circles(struct circle c1, struct circle c2)
{
    //two circles overlap if the distance between centers is less
    //than the sum of radii. ex. (0,0,2) and (3,0,2) overlap
    double x_dist = c1.x - c2.x;
    double y_dist = c1.y - c2.y;
    double sum_radii = c1.radius + c2.radius;

    //rather than take sqrt in  distance formula, we can compare squares
    double dist_squared = x_dist * x_dist + y_dist * y_dist;
    double radii_squared = sum_radii * sum_radii;

    //via the style guide: the simplified way of returning a boolean
    return dist_squared < radii_squared;
}

