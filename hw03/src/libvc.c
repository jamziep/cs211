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
    else if (vc_find_empty(vc)) {
        empty_space -> candidate = strdup_or_else(name);
        empty_space -> count = 0;
        //vc_find_empty(vc) -> candidate = strdup_or_else(name);
        //vc_find_empty(vc) -> count = 0;

        return &empty_space -> count;
        //return &vc_find_empty(vc) -> count;
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
    //initialize a count of vote
    size_t total_count = 0;
    
    //iterate through all the candidates in the list
    for (int ii = 0; ii < MAX_CANDIDATES; ++ii) {

        //if this index of vc has a candidate of NULL,
        //we've reached the end of all candidates to read
        if (vc[ii].candidate == NULL) {
            break;
        }

        //else, add the count of this candidate to total
        total_count += vc[ii].count;
    }
    
    return total_count;
}

//iterates through vc and finds the name of the
//candidate with the most votes. if no candidate
//has more than 0 votes, returns null

const char* vc_max(vote_count_t vc)
{
    ///*
    
    //initialize the max votes observed so far
    //and index where it was found
    size_t curr_max = 0;
    int index_of_max = 0;
    
    //if the first candidate in the array hasn't
    //been initialized, no candidate has >0 votes
    //if (vc[0].candidate = NULL) {
    //  return NULL;
    //}

    for (int ii = 0; ii < MAX_CANDIDATES; ++ii) {

        //if current candidate's name is NULL, we're
        //at the end of the list of candidates
        if (vc[ii].candidate == NULL) {
            continue;
        }

        //else, compare the current candidate's vote
        //count to the max we've observed so far
        if (vc[ii].count > curr_max) {
            curr_max = vc[ii].count;
            index_of_max = ii;
        }
    }

    //use the index of our max vote earner to return
    //the name of their candidate. may be NULL
    //if the array is empty
    return vc[index_of_max].candidate;

    //*/
    
    return NULL;
}

const char* vc_min(vote_count_t vc)
{
    //initialize a count to 0 because we
    //don't know what the first number will be
    size_t curr_min = 0;
    int index_of_min = 0;
    
    //iterate through each candidate in vc and compare
    //its count to the current min
    for (int i = 0; i < MAX_CANDIDATES; ++i) {

        //if the current candidate isn't a valid candidate, skip
        if (vc[i].candidate == NULL) {
            continue;
        }
        
        //if we haven't found the first thing to compare against,
        //initialize it now
        if (curr_min == 0) {    
            curr_min = vc[i].count;
            
        } else if (vc[i].count <= curr_min) {
            //else, let's compare its count to our current max.
            //the <= is bc of the spec where we want the most
            //recent thing we've checked to register as min
            curr_min = vc[i].count;
            index_of_min = i;
                        
        }
    }
    
    //use the index of our min vote earner to return
    //the name of their candidate. may be NULL
    //if the array is empty
    return vc[index_of_min].candidate;

    
    return NULL;
}

void vc_print(vote_count_t vc)
{
    //
    // TODO: your code here
    //

    //iterate through the elements of the array and print
    for (int ii = 0; ii < MAX_CANDIDATES; ++ii) {

        //if this element of array has no data, skip
        if (vc[ii].candidate == NULL) {
            continue;
         }

        //formatting: candidate name left aligned in 20-char
        //column; counts right-aligned in 9-char column
        printf("%-20s %9zu\n", vc[ii].candidate, vc[ii].count);
    }
}


//Returns a pointer to the first element of 'vc' whose
//'candidate' matches 'name', or NULL if not found.
static struct vote_count* vc_find_name(vote_count_t vc, const char* name){
    
    for (size_t ii = 0; ii < MAX_CANDIDATES; ++ii) {

        char*  curr_name = vc[ii].candidate;

        //if curr_name is NULL, means that's not a candidate
        if (!curr_name) {
           //return NULL;
           continue;
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
        fprintf(stderr,"Error in strdup: string not allocated");
        exit(1);
    }

    //put src into new_string on the heap, then return
    //the pointer to it
    strcpy(new_string, src);
    return new_string;
}
