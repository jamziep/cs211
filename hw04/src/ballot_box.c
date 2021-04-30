
#include "ballot_box.h"
#include "helpers.h"

#include <211.h>

#include <stdlib.h>
#include <string.h>

// A `ballot_box_t` (defined in `ballot_box.h`) is a linked list made
// of `struct bb_node`s. Each node owns the next node, so the whole list
// is owned by the pointer to the first node.
struct bb_node
{
    ballot_t ballot;
    struct bb_node* next;
};


// The empty ballot box is the null pointer.
const ballot_box_t empty_ballot_box = NULL;


//Iterates through a linked list of ballot box nodes. Saves a pointer
//to the next thing in the list, frees the current node, and keeps
//freeing nodes until it sees a node where curr_node -> next == NULL
void bb_destroy(ballot_box_t bb)
{
    //null check this ballot box
    if (!bb) {
        return;
    }
    
    ballot_box_t curr_bb_node = bb;
    ballot_box_t next_bb_node = bb -> next;

    while (next_bb_node) {

        //free the node that we just moved on from
        ballot_destroy(curr_bb_node -> ballot);
        free(curr_bb_node);
        
        //"next" node is now the current node
        curr_bb_node = next_bb_node;

        //use a temporary variable to save the pointer to the next
        //item in the list before we free the curr_bb_node
        next_bb_node = curr_bb_node -> next;
    }

    //when we get here, next_bb_node is null, so the only thing
    //left to do is free the final element in the linked list
    ballot_destroy(curr_bb_node -> ballot);
    free(curr_bb_node);
}

void bb_insert(ballot_box_t* bbp, ballot_t ballot)
{
    ballot_box_t head = mallocb(sizeof *head, "bb_insert");
    head->ballot = ballot;
    head->next   = *bbp;
    *bbp = head;
}

//Initializes an instance of ballot_box_t, reads ballots from the infile,
//and keeps reading files until read_ballot() returns NULL
ballot_box_t read_ballot_box(FILE* inf)
{
    ballot_box_t ballot_box = NULL;
    ballot_t ballot = read_ballot(inf);

    while (ballot) {
        bb_insert(&ballot_box, ballot);
        ballot = read_ballot(inf);
    }

    //now that the ballot is not in use, destroy it
    ballot_destroy(ballot);
    
    //ballots have been read into our ballot box as linked list
    return ballot_box;
}

// starts a vc, reads ballots from bb and updates the vc accordingly.
// Also adds to the count. keeps going until the ballot box is empty
vote_count_t bb_count(ballot_box_t bb)
{
    // keep in mind that YOU the caller have to destroy this vc_count_t
    vote_count_t result = vc_create();
    // null check for the result
    if(!result){
        exit(33);
    }

    ballot_box_t curr_bb = bb;
    ballot_box_t next_bb = curr_bb -> next;

    //iterate through ballot box
    while(1){
        ballot_t curr_ballot = curr_bb -> ballot;
        // calls count_ballot, which uses vc_update
        // to increment the count pointer of the ballot
        count_ballot(result, curr_ballot);

        if(!next_bb){
            break;
        }

        // set next ballot as current and next as next
        curr_bb = next_bb;
        next_bb = curr_bb -> next;             
    }

    return result;
}

//iterates through the linked list of the ballot box. from there,
//goes into the array of entries on each ballot. if the name of an entry
//matches the arg "candidate", "crosses off" a candidate by
//marking that entry as inactive
void bb_eliminate(ballot_box_t bb, const char* candidate)
{
    //if ballot box is null, do nothing
    if (!bb) {
        return;
    }
  
    ballot_box_t curr_bbox = bb;
    ballot_box_t next_bbox = curr_bbox -> next;

    for (;;){

        //go into current ballot and if an element in its entries
        //matches "candidate", mark it as inactive
        ballot_t ballot = curr_bbox -> ballot;
        ballot_eliminate(ballot, candidate);

        //keep checking elements of list until next_bbox == NULL
        if (!next_bbox) {
            break;
        }

        //move on to the next items in the list
        curr_bbox = next_bbox;
        next_bbox = curr_bbox -> next;  
    }    
}

char* get_irv_winner(ballot_box_t bb)
{
    //algorithm:
    // -start with empty vote count map, count every ballot in bb
    // -Check to see if leading candidate has majority. If true,
    // then we are done
    // -If no votes were cast then there is no winner, so the result is NULL
    // -Otherwise, candidate in last place is eliminated.

    //things I've tried for testing: initialize variables with types outside
    //loop; break and (destroy + return) stuff after the loop; use a bool
    //called "result_found" to regulate when the loop ends. break statements
    //might nullify changes to variables that happen in a loop (my theory)

    //null check the ballot box
    if (!bb) {
        return NULL;
    }
    
    for(;;){
        
        // bb_count will initialize the vc for us
        vote_count_t vc = bb_count(bb);

        const char* leader = vc_max(vc);
        const char* loser = vc_min(vc);
        size_t vtotal = vc_total(vc);

        //check to see if leading cand has majority. If true, return candidate
        if(vc_lookup(vc, leader) > (vtotal/2)){
            char* result = strdupb(leader, "get_irv_winner");
            vc_destroy(vc);
            return result;

        }else if(vtotal == 0){
            // if there are no votes cast, then the result is NULL
            char* result = NULL;
            vc_destroy(vc);
            return result;

        }else{
            // otherwise eliminate the last place candidate.
            bb_eliminate(bb, loser);
            vc_destroy(vc);
        }
    }
}
