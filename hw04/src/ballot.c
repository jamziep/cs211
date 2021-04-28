#include "ballot.h"
#include "helpers.h"

#include <211.h>

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// A `ballot_t` (defined in `ballot.h`) is be a pointer to a
// heap-allocated `struct ballot`, with the following invariant:
//
//  - `length <= MAX_CANDIDATES`
//
//  - the first `length` elements of `entries` are initialized
//
//  - the `name` field of each of the first `length` entries is non-NULL
//    and points to an OWNED, heap-allocated string containing only
//    letters, all of which are uppercase (a la isupper(3)).
//
// The remaining elements of `entries` (`MAX_CANDIDATES - length`)
// should be considered uninitialized.

struct entry
{
    char* name;
    bool active;
};

struct ballot
{
    size_t length;
    struct entry entries[MAX_CANDIDATES];
};


ballot_t ballot_create(void)
{
    //first malloc an array of MAX_CANDIDATES entries, then null check
    ballot_t ballot = malloc(MAX_CANDIDATES * sizeof(struct entry));
    if (!ballot) {
        exit(2);
    }

    //invariant: the length of the ballot, which starts out at 0
    ballot -> length = 0;
    
    //no need to allocate the names of candidates in the ballot; those
    //come in ballot_insert()
    return ballot;
}

void ballot_destroy(ballot_t ballot)
{
    //first: null check the ballot
    if (!ballot){
        return;
    }

    //deallocate every name associated with the ballot.
    //this means checking the length of the ballot
    size_t ballot_length = ballot -> length;
    for (size_t ii = 0; ii < ballot_length; ++ii) {
        free(ballot -> entries[ii].name);
    }

    //free the array itself
    free(ballot);
}

void ballot_insert(ballot_t ballot, char* name)
{
    //
    // TODO: your code here
    //

    //standardize the name. clean_name() returns void so
    //modify the name in place
    //though idk if you can do that in the stack or if it
    //has to be in the heap already
    clean_name(name);

    //add the name to an entry and add that to the ballot
    size_t ballot_length = ballot -> length;

    //access the index equal to the ballot length. for example if length
    //is 1, the previous index is at index 0 so we'll add to index 1

    //I may need to malloc the name in memory, so I'll try that
    //ballot -> entries[ballot_length].name = name;
    ballot -> entries[ballot_length].name = malloc(sizeof name);
    ballot -> entries[ballot_length].name = name;

    //increase the length of the ballot by 1 and set that entry to active
    ballot -> entries[ballot_length].active = true;
    ballot -> length += 1;
}


//returns the name of the first still-active candidate, or NULL
//if no active candidates remain
const char* ballot_leader(ballot_t ballot)
{
    //
    // TODO: replace with your code:
 
    //iterate through the ballot and find the first candidate
    //whose "active" flag is true--the highest-rated remaining
    size_t ballot_length = ballot -> length;

    for (size_t ii; ii < ballot_length; ++ii) {

        //check current entry and see if "active" == true. if so,
        //this is the leader; return their name
        if (ballot -> entries[ii].active) {
            return ballot -> entries[ii].name;
        }
    }
    
    //if we've gotten here, means no remaining active candidates
    //were found
    return NULL;
}

void ballot_eliminate(ballot_t ballot, const char* name)
{
    //
    // TODO: your code here
    //
<<<<<<< HEAD

    // algo idea so I don't forget: scan through the entire ballot, match the candidate name, when it matches, eliminate.
    
=======
    //iterate through the ballot. if you find an entry that
    //matches the name given to this function, mark it as inactive

    size_t ballot_length = ballot -> length;
    for (size_t ii = 0; ii < ballot_length; ++ii) {

        char* curr_name = ballot -> entries[ii].name;
        if (strcmp(curr_name, name) == 0) {
            ballot -> entries[ii].active = false;
            return;
        }
    }
>>>>>>> e576fae45110482a41a023b1727b591906d04e3e
}

void count_ballot(vote_count_t vc, ballot_t ballot)
{
    //
    // TODO: your code here
    //Lance
}

ballot_t read_ballot(FILE* inf)
{
    //
    // TODO: replace with your code:
    //

    //read in the first thing from inf
    char* name = fread_line(inf);

    //null-check inf to see if it has nothing left to read
    if (!name) {
        return NULL;
    }


    //create a ballot to read this stuff into
    ballot_t ballot = ballot_create();

    //reads in the input from the command line, one line at a time,
    //until it sees EOF or a % sign on a single line

    //figure out a way to compare a string to EOF
    while (/*name != EOF &&*/ strcmp(name,"%") != 0) {

        //insert the name into the ballot. clean_name()
        //occurs inside ballot_insert()

        //does this have to take into account the size of max_candidates
        //to see if we can add a candidate to the ballot here?
        ballot_insert(ballot, name);
        name = fread_line(inf);
    }
    
    //at this point, we're done filling in the ballot and should return it

    //NOTE: this may not be the ideal way to go about this. the way this
    //runs, the null-check only sees if inf has nothing left to read in that
    //it returns null. however, inf can be EOF or a %, in which case
    //we initialize an empty ballot. check on this during testing
    return ballot;
}

void clean_name(char* name)
{
    size_t src = 0;
    size_t dst = 0;

    while(name[src] != 0){
        if(isalpha(name[src])){
            if(islower(name[src])){
                name[dst] = toupper(name[src]);
                ++dst;
            
            }else if(isupper(name[src])){
                name[dst] = name[src];
                ++dst;          
            
            }
        }
        ++src;
    }
    
    name[dst] = 0;
}

void print_ballot(FILE* outf, ballot_t ballot)
{
    for (size_t i = 0; i < ballot->length; ++i) {
        bool active = ballot->entries[i].active;
        fprintf(outf, "%c%s%s\n",
                active? ' ' : '[',
                ballot->entries[i].name,
                active? "" : "]");
    }
}
