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


//helper functions, written below
static struct vote_count* vc_find_name(vote_count_t vc, const char* name);
static struct vote_count* vc_find_empty(vote_count_t vc);
static char* strdup_or_else(const char* src);



vote_count_t vc_create(void)
{
    //malloc an array of MAX_CANDIDATES * 'struct vote_counts'
    vote_count_t vc = malloc( MAX_CANDIDATES * sizeof(struct vote_count));
  
    //if memory allocation fails, return a null pointer
    if (!vc) {
        return NULL;
    }

    //initialize names of each item in the array. value should be null,
    for (size_t ii = 0; ii < MAX_CANDIDATES; ++ii) {
    
        //we don't need malloc here bc of strdup function
        //vc[counter].candidate = malloc(sizeof(char*));
        vc[ii].candidate = NULL;
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


//return the pointer to the "count" of type size_t
//of the vote count map that matches the "name" string
size_t* vc_update(vote_count_t vc, const char *name)
{
    //if the name exists in array, returns the pointer to the
    //element of vc that has this name for candidate.
    vote_count_t name_location = vc_find_name(vc, name);

    //if an empty space exists in array, returns the pointer
    //to the element of vc that has the empty space
    vote_count_t empty_space = vc_find_empty(vc);

    //if location is not null, return the pointer to its count
    if (name_location){
        return &name_location -> count;
    }

    //else, if the name doesn't exist in the array yet,
    //add the name to the array and initialize count to 0
    else if (empty_space) {
        empty_space -> candidate = strdup_or_else(name);
        empty_space -> count = 0;

        return &empty_space -> count;
    }
   
    //if we got here, means a match wasn't found
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


//Returns a pointer to the first element of 'vc' whose
//'candidate' matches 'name', or NULL if not found.
static struct vote_count* vc_find_name(vote_count_t vc, const char* name){
    
    for (size_t ii = 0; ii < MAX_CANDIDATES; ++ii) {

        char* curr_name = vc[ii].candidate;

        //if curr_name is NULL, means we've reached the end of data
        if (!curr_name) {
            return NULL;
        }
        
        if (strcmp(curr_name, name) == 0 ) {
            //if we've found a match for the desired name
            //in the array of vote_count objects,
            //return the pointer to that element
            return &vc[ii];
        }     
    }
    
    //else, if we've gotten here, array is full
    return NULL;
}


//Returns a pointer to the first element of 'vc' whose
//'candidate' is NULL, or NULL if it's full
static struct vote_count* vc_find_empty(vote_count_t vc) {

    for (size_t ii = 0; ii < MAX_CANDIDATES; ++ii) {

        char* curr_name = vc[ii].candidate;

        //if curr_name is null, return a pointer to that element
        if (!curr_name) {
            return &vc[ii];
        }   
    }

    //else, if we've gotten here, the array is full
    return NULL;
}


//Clones a string onto the heap, printing a message
//to stderr and exiting with code 1 if malloc() fails
static char* strdup_or_else(const char* src) {

    //allocate a new variable the size of the src
    char* new_string = malloc(strlen(src) + 1);

    //null-check the result
    if (!new_string) {
        return NULL;
    }

    //else, return the pointer to this new string
    return new_string;
}
