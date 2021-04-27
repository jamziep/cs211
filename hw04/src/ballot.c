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
    
    
}

const char* ballot_leader(ballot_t ballot)
{
    //
    // TODO: replace with your code:
    //
    return NULL;
}

void ballot_eliminate(ballot_t ballot, const char* name)
{
    //
    // TODO: your code here
    //
}

void count_ballot(vote_count_t vc, ballot_t ballot)
{
    //
    // TODO: your code here
    //
}

ballot_t read_ballot(FILE* inf)
{
    //
    // TODO: replace with your code:
    //
    return NULL;
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
