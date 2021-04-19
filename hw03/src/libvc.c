#include "libvc.h"

#include <211.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    vote_count_t candidates = malloc( MAX_CANDIDATES * sizeof(vote_count_t));

    //or just malloc one instance of vote_count_t?
    //vote_count_t candidate = malloc( sizeof(vote_count_t));
    
    //if memory allocation fails, return a null pointer so it can be
    //freed safely
    if (!candidates) {
        //printf("\n\nThis ptocess failed");
        return NULL;
    }
    
    //if memory allocation went well
    return candidates;
}

void vc_destroy(vote_count_t vc)
{
    //vc can be null; but generally vc_destroy() will
    //free the pointer associated with vc

    //first: free the string name associated with this
    //instance of vote_count_t, if it has a name
    //the "&vc" is so we can access the vote_count object
    //at the pointer vc, from which we get the pointer to
    //candidate name

    //note: this is valid syntax on its own, but freeing
    //this and then the vc itself caused a double free
    /*
    if (&vc -> candidate) {
        //if its candidate category is not a NULL pointer
        free(&vc -> candidate);
    }
    */
    
    //next: free the instance of vote_count_t itself
    free(vc);
    //caused an address sanitizer error: double-free
    //I think the issue is that the pointer to the candidate
    //name, &vc -> candidate, is owned by vc, so freeing
    //the name and then freeing the owner causes a double-free
}
size_t* vc_update(vote_count_t vc, const char *name)
{
    //
    // TODO: replace with your code:
    //

    //return the pointer to the "count" of type size_t
    //of the vote count map that matches the "name" string

    //iterate through the candidates array and find one that
    //matches this name. start with a pointer to the first index
    vote_count_t selected_vc = vc;

    //use this counter to make sure we don't go out of bounds
    size_t counter = 0;

    
    while (selected_vc && counter < MAX_CANDIDATES) {
        //while the current pointer is not a null pointer

        //if the candidate name of this item in array matches
        //"name", break out of the loop
        //removed the & in front of selected_vc so as to keep
        //the type of selected_name as char* and not char**

        //this is causing heap buffer overflow: 0 bytes out of
        //bound of the 128-byte (sizeof vote_count_t * 16) region
        //for each iteration of loop, this line is invalid
        char** selected_name = &selected_vc -> candidate;

        //currently this is reading selected_name as a null pointer
        //group these in one if statement later; this is just for debug
        
        //if (selected_name){ //this part is working
        //    if ( //strcmp(selected_name, "test") == 0 && //this part isn't
        //        strcmp("test", name) == 0) {
        //     break;
        //  }
        //}
        
        //increment to the next vote_count object in the array
        ++vc;
        ++counter;
        
        selected_vc = vc;        
    }

    
    //note: adding in this iterating through a loop caused an ASan error.
    //see the vc_update_asan_error to see what this looked like

    //can probably fix this with what they're talking about in the pdf:
    //if name isn't present in vc yet, extend vc to map "name"
    //and initialize that element's count to 0


    
    
    //first, assume that we've already found the vote_count
    //map that matches this name. this will have to be modified later
    //if the selected_vc pointer is not NULL and the pointer to its
    //count is not null
    if (selected_vc && &selected_vc -> count) {
        //if the pointer to the count is not none

        //find the value, then get a pointer to it
        size_t* count_pointer = &selected_vc -> count;
        //changing vc to &vc caused an address sanitizer
        //error: heap buffer overflow

        //fixed by changing initialization from a single
        //instance of vote_count_t to an array of instances
        return count_pointer;
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
