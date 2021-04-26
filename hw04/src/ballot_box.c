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


void bb_destroy(ballot_box_t bb)
{
    //
    // TODO: your code here
    //
}

void bb_insert(ballot_box_t* bbp, ballot_t ballot)
{
    ballot_box_t head = mallocb(sizeof *head, "bb_insert");
    head->ballot = ballot;
    head->next   = *bbp;
    *bbp = head;
}

ballot_box_t read_ballot_box(FILE* inf)
{
    //
    // TODO: replace with your code:
    //
    return empty_ballot_box;
}

vote_count_t bb_count(ballot_box_t bb)
{
    vote_count_t result = vc_create();

    //
    // TODO: your code here
    //

    return result;
}

void bb_eliminate(ballot_box_t bb, const char* candidate)
{
    //
    // TODO: your code here
    //
}

char* get_irv_winner(ballot_box_t bb)
{
    //
    // TODO: replace with your code:
    //
    char* result = strdupb("FIXME", "get_irv_winner");
    //
    // TODO: your code here too
    //
    return result;
}
