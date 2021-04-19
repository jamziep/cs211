#include "libvc.h"

#include <211.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//for determining if a matching name has been found
#include <stdbool.h>

// This definition is private to this file; code in all other files
// can only handle pointers to it: values of type `struct vote_count*`
// (a/k/a `vote_count_t`), not values of type `struct vote_count`. Only
// code in this file sees this definition, so only code in this file can
// work with `struct vote_count`s directly.
struct vote_count
{
    char*  candidate;
    size_t count;
};

// A vote_count_t will be a pointer to a `malloc`ed array of
// MAX_CANDIDATES `struct vote_count`s, with the following invariant:
//
//  - Some prefix of `candidate` fields are distinct, OWNED, `malloc`ed
//  strings, and the remaining `candidate` fields are NULL.
//
//  - When a `candidate` field is NULL the value of the corresponding
//  `count` does not mean anything, but when the `candidate` is non-NULL
//  then the `count` must be a properly initialized count.

/*
 * Q: Where are header comments for the following functions?
 * A: libvc.h
 */

vote_count_t vc_create(void)
{
    //malloc an array of MAX_CANDIDATES * 'struct vote_counts'
    vote_count_t vc = malloc( MAX_CANDIDATES * sizeof(struct vote_count));
  
    //if memory allocation fails, return a null pointer
    if (!vc) {
        return NULL;
    }

    //initialize names of each item in the array. value should be null,
    //but use malloc() to allocate size to the names
    size_t counter = 0;
    while (counter < MAX_CANDIDATES) {

        //set name of current candidate to null
        vc[counter].candidate = malloc(sizeof(char*));
        vc[counter].candidate = NULL;
        ++counter;
    }
    
    //if memory allocation went well
    return vc;
}

void vc_destroy(vote_count_t vc)
{
    //first: free the string name associated with each
    //instance of struct vote_count, if it has a name
   
    size_t counter = 0;
    while (counter < MAX_CANDIDATES) {

        //if the candidate's name is not null, free it
        if (vc[counter].candidate) {
            free(vc[counter].candidate);
        }
        counter++;
    }
    
    //if vc is not null, free it
    if (vc) {
        free(vc);
    }
}

size_t* vc_update(vote_count_t vc, const char *name)
{
    bool matching_name = false;
    //return the pointer to the "count" of type size_t
    //of the vote count map that matches the "name" string
    
    //use this counter to make sure we don't go out of bounds
    size_t counter = 0;

    //iterate through the candidates array and find one that
    //matches this name. start with 0 and go to MAX_CANDIDATES
    while ( counter < MAX_CANDIDATES) {

        struct vote_count curr_object = vc[counter];
        char* curr_name = curr_object.candidate;

        //if curr_name is null, we're at the end of the array
        //that has been filled in. extend vc to map "name"
        //and initialize count to zero
        if (!curr_name) {
            vc[counter].candidate = name;
            //this is where I'll want to do the strdup thing
            vc[counter].count = 0;
            break;
        }
        
        if (strcmp(curr_name, name) == 0 ) {
            //if we've found a match for the desired name
            //in the array of vote_count objects,
            //return the pointer to its count and break out
            matching_name = true;
            return &curr_object.count;
        }

        //increment to the next vote_count object in the array
        ++counter;       
    }
    
    //can probably fix this with what they're talking about in the pdf:
    //if name isn't present in vc yet, extend vc to map "name"
    //and initialize that element's count to 0

    //i.e. if we broke out of the loop, and didn't exceed the allowed
    //number of candidates, let's make a new candidate at the first
    //position with a name of null
    if (counter < MAX_CANDIDATES) {
        
    }

    return NULL;
}

size_t vc_lookup(vote_count_t vc, const char* name)
{
    //
    // TODO: replace with your code:
    //
    return 0;
}

size_t vc_total(vote_count_t vc)
{
    //
    // TODO: replace with your code:
    //
    return 0;
}

const char* vc_max(vote_count_t vc)
{
    //
    // TODO: replace with your code:
    //
    return NULL;
}

const char* vc_min(vote_count_t vc)
{
    //
    // TODO: replace with your code:
    //
    return NULL;
}

void vc_print(vote_count_t vc)
{
    //
    // TODO: your code here
    //
}
