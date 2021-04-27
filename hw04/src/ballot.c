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

//Initializes a ballot, i.e. the ranked choices of all voters.
//Contains an array of instances of struct "entry" for each candidate
//name that the user gives. "length" keeps track of how many
//spaces in the array are currently in use.
ballot_t ballot_create(void)
{
    //
    // TODO: replace with your code:
    //

    //malloc an array of MAX_CANDIDATES antries and null check
    ballot_t ballot =  malloc(MAX_CANDIDATES * sizeof(struct entry));
    if (!ballot) {
        return NULL;
    }
                     
    return NULL;
}

void ballot_destroy(ballot_t ballot)
{
    //
    // TODO: your code here
    //
}

void ballot_insert(ballot_t ballot, char* name)
{
    //
    // TODO: your code here
    //
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
    //
    // TODO: your code here
    //
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
