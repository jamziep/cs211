#include "translate.h"

#include <211.h>

#include <stdio.h>
#include <string.h>


#define USAGE_MESSAGE   "Usage: %s FROM TO < INPUT_FILE\n"
#define LENGTH_MESSAGE  "%s: error: lengths of FROM and TO differ\n"
#define OOM_MESSAGE     "%s: error: out of memory\n"


int main(int argc, char* argv[])
{
    if (argc != 3) {
        fprintf(stderr, USAGE_MESSAGE, argv[0]);
        return 1;
    }

    //
    // TODO: Your code goes here
    //
    
    char* from = expand_charseq(argv[1]);
    char* to   = expand_charseq(argv[2]);
    char* line;

    //check to see if either of the memory allocations
    //returned null, then return the out_of_memory msg
    if (!from || !to) {
        fprintf(stderr, OOM_MESSAGE, argv[0]);
        return 10;
    }
    
    //check the size of from and to
    if (strlen(from) != strlen(to) ) {
        fprintf(stderr, LENGTH_MESSAGE, argv[0]);

        //try freeing from and to, since we don't need it
        free(from);
        free(to);
        return 2;
    }
    
    while ((line = read_line()) != NULL) {
        translate(line, from, to);
        printf("%s\n", line);
        free(line);
    }

    free(to);
    free(from);
}
