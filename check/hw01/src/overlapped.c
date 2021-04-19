// overlapped - Reads a target circle, then any number of candidate
// circles. Prints "overlapped\n" for each candidate circle that
// intersects the target and "not overlapped\n" for each candidate
// circle that does not. Exits after reading an invalid circle.

#include "circle.h"
#include <stdio.h>

#define OVERLAPPED_MSG      "overlapped\n"
#define NOT_OVERLAPPED_MSG  "not overlapped\n"

int main(void)
{
    //target circle: the thing we compare our candidate circles to
    struct circle target = read_circle();

    //check to see if target is valid. if not, exit program
    if (! valid_circle(target) ) {
        return 1;
    }

    //infinite loop: user can provide as many candidate circles
    //as they want. exit w/ an invalid circle
    for (;;) {
        struct circle candidate = read_circle();

        //check if valid. exit code 0 is per the instructions
        if (! valid_circle(candidate)) {
            return 0;
        }

        //check if overlapped. if so, print that msg; else, print other msg
        if (overlapped_circles(target,candidate)) {
            printf(OVERLAPPED_MSG);
        } else {
            printf(NOT_OVERLAPPED_MSG);
        }
        
    } //matches for(;;)
}
