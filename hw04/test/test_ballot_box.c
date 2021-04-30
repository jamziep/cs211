///
/// Tests for functions in ../src/ballot_box.c.
///

#include "ballot_box.h"
#include "helpers.h"

#include <211.h>

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>


///
/// FORWARD DECLARATIONS
///

// Takes the expected winner, followed by all the votes. Each ballot is
// given as successive string arguments, terminated by "%". The final
// argument, after the final ballot's "%", must be NULL.
static void check_election(const char* expected_winner, ...);

// Test case functions (you need more!):
static void three_candidates_tied(void),
            win_on_third_round(void),
            example_from_wikipedia(void);

// Lance's functions
static void no_candidates(void);
static void single_cand_ballots(void);
ballot_t read_ballot_from_file(const char* file_path);
static void uneven_ballot(void);
static void bogus_ballot1(void);
static void bogus_ballot2(void);

    


///
/// MAIN FUNCTION
///

int main(void)
{
   three_candidates_tied();
   win_on_third_round();
   example_from_wikipedia();
   single_cand_ballots();
   no_candidates();
   uneven_ballot();
   bogus_ballot1();
   bogus_ballot2();
       
}

///
/// TEST CASE FUNCTIONS
///

static void no_candidates(void)
{
    if (MAX_CANDIDATES < 3){
        return;
    }

    check_election(NULL, "%", NULL);
}

static void single_cand_ballots(void)
{
    if (MAX_CANDIDATES < 3){
        return;
    }

    check_election("B",
                   "A","%",
                   "B","%",
                   "C","%",
                   "B","%",
                   NULL);
    
}

static void uneven_ballot(void)
{
    if (MAX_CANDIDATES < 3) {
        return;
    }

    check_election("A",
                   "a", "b", "c", "d", "%"
                   "a", "c", "b", "%",
                   "c", "a", "b", "%",
                   "a", "b", "c", "%",
                   NULL);
        

}

static void three_candidates_tied(void)
{
    if (MAX_CANDIDATES < 3) {
        return;
    }

    check_election("A",
            "a", "b", "c", "%",
            "a", "b", "c", "%",
            NULL);
}

static void win_on_third_round(void)
{
    if (MAX_CANDIDATES < 6) {
        return;
    }

    check_election("BIDEN",
            "Bloomberg", "Biden", "%",
            "Yang", "Biden", "%",
            "Warren", "Biden", "%",
            "Sanders", "Biden", "%",
            "Biden", "Sanders", "%",
            "Biden", "Gabbard", "%",
            NULL);
}

static void example_from_wikipedia(void)
{
    if (MAX_CANDIDATES < 3) {
        return;
    }

    check_election("SUE",
            "bob", "bill", "s u e", "%",
            "Sue", "Bob", "Bill", "%",
            "Bill!", "Sue!", "BoB!", "%",
            "bob", "bill", "sue", "%",
            "sue", "bob", "bill", "%",
            NULL);
}

static void bogus_ballot1(void)
{
    if (MAX_CANDIDATES < 3){
        return;
    }

    check_election("JOE",
                   "kanye", "kanye", "kanye", "kanye", "kanye", "%",
                   "Joe", "%",
                   "Joe", "%",
                   "Joe", "%",
                   NULL);
}

static void bogus_ballot2(void)
{
    if (MAX_CANDIDATES < 3){
        return;
    }

    check_election("",
                   "!@&?", "....@#!" ,"&^%)" ,"%"
                   "     " , "\n)(", "%",
                   NULL);
}






//functions that need to be tested:
//bb_destroy()
//bb_insert()
//read_ballot_box()
//bb_count()
//bb_eliminate()
//get_irv_winner()
//
//
///
/// HELPER FUNCTIONS YOU SHOULD USE
///

// These are helpers for `check_election()` that you are unlikely to
// call directly.
static ballot_box_t build_ballot_box(va_list);
static void check_irv_winner(const char*, ballot_box_t);


// Simulates an election and checks the winner. The first argument
// is the name of the expected winner, and the remaining arguments
// are the names on the ballots, with "%" in between and a NULL
// marking the end. (See uses of this function above for examples.)
static void
check_election(const char* expected, ...)
{
    va_list ap;
    va_start(ap, expected);
    ballot_box_t bb = build_ballot_box(ap);
    va_end(ap);

    check_irv_winner(expected, bb);

    bb_destroy(bb);
}


/// HELPERS FOR THE HELPERS

// Builds a ballot box from multiple string arguments. Returns
// ownership of the ballot box to the caller.
static ballot_box_t
build_ballot_box(va_list ap)
{
    ballot_box_t bb = empty_ballot_box;
    ballot_t ballot = NULL;

    char* name;
    while ((name = va_arg(ap, char*))) {
        if (!ballot) {
            ballot = ballot_create();
        }

        if (strcmp(name, "%") == 0) {
            bb_insert(&bb, ballot);
            ballot = NULL;
        } else {
            ballot_insert(ballot, strdupb(name, "check_election"));
        }
    }

    if (ballot) {
        bb_insert(&bb, ballot);
    }

    return bb;
}

// Checks that `expected` is the winner of `bb`. Borrows both arguments.
static void
check_irv_winner(const char* expected, ballot_box_t bb)
{
    char* actual = get_irv_winner(bb);

    if (expected) {
        CHECK_STRING(actual, expected);
    } else {
        CHECK_POINTER(actual, expected);
    }

    free(actual);
}

// reading ballots from files (same as test/test_ballot.c)
ballot_t read_ballot_from_file(const char* file_path)
{
    FILE* ballot_file = fopen(file_path, "r");
    if (!ballot_file){
        fclose(ballot_file);
        return NULL;
    }

    ballot_t ballot = read_ballot(ballot_file);

    fclose(ballot_file);
    return ballot;
}
