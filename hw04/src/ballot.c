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

//Standardizes a name, adds it to an entry, and adds that entry to
//the ballot. Also increments the length of the ballot by 1
void ballot_insert(ballot_t ballot, char* name)
{
    clean_name(name);
    size_t ballot_length = ballot -> length;
    ballot -> entries[ballot_length].name = strdupb(name, "ballot_insert");
    
    //increase the length of the ballot by 1 and set that entry to active
    ballot -> entries[ballot_length].active = true;
    ballot -> length += 1;

    //free "name" from the heap
    free(name);
}


//iterate through the ballot and find the first candidate whose "active"
//flag is true--the highest-rated remaining, and returns their name
//returns NULL if no leader found
const char* ballot_leader(ballot_t ballot)
{
    size_t ballot_length = ballot -> length;
    for (size_t ii = 0; ii < ballot_length; ++ii) {

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
<<<<<<< HEAD

=======
>>>>>>> 0629e6b01de3688dd4ed03d1da5c70860380903b
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
}

void count_ballot(vote_count_t vc, ballot_t ballot)
{
    //
    // TODO: your code here
    //Lance
    char* name = ballot -> entries[0].name;

    // updates the vc with the current name
    size_t* count_ptr = vc_update(vc, name);

    // vc_update returns null, exit with error code 4
    if(!count_ptr){
        exit(4);
    }

    *count_ptr += 1;
          
}

//Reads in candidate names from the command line, one line at a time,
//until it sees EOF (which equals NULL) or a % sign on a single line.
//Creates an instance of "ballot" from the names it reads in.
ballot_t read_ballot(FILE* inf)
{
    char* name = fread_line(inf);

    //if user doesn't give any names, make no ballot
    if (!name || strcmp(name,"%") == 0) {
        return NULL;
    }

    //create a ballot to read this stuff into. null check to see
    //if memory allocation fails
    ballot_t ballot = ballot_create();
    if (!ballot) {
        exit(12);
    }

    //check the number of candidates read in, and compare it to
    //max_candidates
    size_t num_candidates_inserted = 0;
    
    //"while "name" is not NULL (i.e. EOF) and "name is not a %":
    while (name && strcmp(name,"%") != 0) {

        //if we've exceeded max candidates, don't do anything with
        //what we just read in from fread_line()
        if (num_candidates_inserted >= MAX_CANDIDATES) {
            exit(3);
        }
        
        //insert the name into the ballot
        clean_name(name);
        ballot_insert(ballot, name);
        ++num_candidates_inserted;
        
        //free the thing returned by fread_line() then get ready to
        //read in a new string of unknown size from file
        free(name);
        name = fread_line(inf);
    }

    //from the manual, the pointer returned by fread_line must be freed
    free(name);
    
    //at this point, we're done filling in the ballot and should return it
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
