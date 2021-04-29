///
/// Tests for functions in ../src/ballot.c.
///

#include "ballot.h"
#include "libvc.h"
#include "helpers.h"

#include <211.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>


///
/// FORWARD DECLARATIONS
///

// Checks that `clean_name(name)` produces `expected`. (Borrows both
// arguments.)
static void check_clean_name(const char* name, const char* expected);

// Test case functions (you need more!):
static void test_clean_name(void);
static void test_ballot_3(void);
static void test_ballot_with_vc(void);

//our functions
static void test_ballot_create_destroy(void);


///
/// MAIN FUNCTION
///

int main(int argc, char* argv[])
{
    test_clean_name();
    test_ballot_3();
    test_ballot_with_vc();
    test_ballot_create_destroy();
}


///
/// TEST CASE FUNCTIONS
///

static void test_clean_name(void)
{
    check_clean_name("Alan Turing", "ALANTURING");
    check_clean_name("Stroustrup, Bjarne", "STROUSTRUPBJARNE");
    check_clean_name("C4P0", "CP");
    check_clean_name("    ", "");
    check_clean_name("Jeff Bezos", "JEFFBEZOS");
    check_clean_name("!1423@!", "");
}

static void test_ballot_3(void)
{
    // Skip this test if MAX_CANDIDATES isn't large enough.
    if (MAX_CANDIDATES < 3) {
        return;
    }

    ballot_t ballot = ballot_create();
    CHECK( ballot != NULL );
    if (ballot == NULL) {
        // Can't keep testing if ballot is NULL
        return;
    }

    CHECK_POINTER(ballot_leader(ballot), NULL);

    ballot_insert(ballot, strdupb("A", "test_ballot_3"));
    ballot_insert(ballot, strdupb("B", "test_ballot_3"));
    ballot_insert(ballot, strdupb("C", "test_ballot_3"));

    CHECK_STRING(ballot_leader(ballot), "A");
    ballot_eliminate(ballot, "D");
    CHECK_STRING(ballot_leader(ballot), "A");
    ballot_eliminate(ballot, "B");
    CHECK_STRING(ballot_leader(ballot), "A");
    ballot_eliminate(ballot, "A");
    CHECK_STRING(ballot_leader(ballot), "C");
    ballot_eliminate(ballot, "C");
    CHECK_POINTER(ballot_leader(ballot), NULL);
    ballot_eliminate(ballot, "C");
    CHECK_POINTER(ballot_leader(ballot), NULL);

    ballot_destroy(ballot);
}

static void test_ballot_with_vc(void)
{
    //initialize a vote_count_t
    vote_count_t vote_count = vc_create();

    //create a ballot that ranks three candidates, A B + C
    ballot_t ballot1 = ballot_create();
    ballot_insert(ballot1, strdupb("A", "test_with_vc"));
    ballot_insert(ballot1, strdupb("B", "test_with_vc"));
    ballot_insert(ballot1, strdupb("C", "test_with_vc"));

    //check the current leader of the ballot and add 1 to the
    //count for leader, A
    const char* leader1 = ballot_leader(ballot1);
    CHECK_STRING( leader1, "A");
    size_t* cp = vc_update(vote_count, leader1);
    *cp += 1;
    
    //verify the counts in vc
    CHECK_SIZE( vc_lookup(vote_count,"A"), 1);
    CHECK_SIZE( vc_lookup(vote_count,"B"), 0);
    CHECK_SIZE( vc_lookup(vote_count,"C"), 0);

    //count again and confirm the votes
    leader1 = ballot_leader(ballot1);
    cp = vc_update(vote_count, leader1);
    *cp += 1;

    CHECK_SIZE( vc_lookup(vote_count,"A"), 2);
    CHECK_SIZE( vc_lookup(vote_count,"B"), 0);
    CHECK_SIZE( vc_lookup(vote_count,"C"), 0);

    //eliminate candidate B, count again, and confirm the votes
    ballot_eliminate( ballot1, "B" );
    leader1 = ballot_leader(ballot1);
    CHECK_STRING( leader1, "A");
    
    cp = vc_update(vote_count, leader1);
    *cp += 1;

    CHECK_SIZE( vc_lookup(vote_count,"A"), 3);
    CHECK_SIZE( vc_lookup(vote_count,"B"), 0);
    CHECK_SIZE( vc_lookup(vote_count,"C"), 0);

    //eliminate A, and confirm that a vote goes toward C
    ballot_eliminate( ballot1, "A" );
    leader1 = ballot_leader(ballot1);
    CHECK_STRING( leader1, "C");

    cp = vc_update(vote_count, leader1);
    *cp += 1;

    CHECK_SIZE( vc_lookup(vote_count,"A"), 3);
    CHECK_SIZE( vc_lookup(vote_count,"B"), 0);
    CHECK_SIZE( vc_lookup(vote_count,"C"), 1);

    //eliminate C and confirm that counting the ballot again
    //has no effect on the counts
    ballot_eliminate( ballot1, "C");
    CHECK_POINTER( ballot_leader(ballot1), NULL);


    
    ballot_destroy(ballot1);
    vc_destroy(vote_count);
}


///
/// HELPER FUNCTIONS
///

// Asserts that `clean_name(name)` returns `expected`.
static void check_clean_name(const char* name, const char* expected)
{
    char* actual = strdupb(name, "check_clean_name");
    clean_name(actual);
    CHECK_STRING(actual, expected);
    free(actual);
}


//check our functions
static void test_ballot_create_destroy(void) {

    ballot_t ballot = ballot_create();
    ballot_destroy(ballot);

}

//functions to test:
//ballot_insert()
//ballot_leader()
//ballot_eliminate()
//count_ballot()
//read_ballot()
//clean_name()
//

//testing functions to use:
//CHECK_POINTER() for pointers
//CHECK_STRING() for strings
//CHECK_SIZE() for numerical values
//CHECK( ) for logical true/false
//
//

