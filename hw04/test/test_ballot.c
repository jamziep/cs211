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
    //test_clean_name();
    //test_ballot_3();
    //test_ballot_with_vc();
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
    //
    // TODO: your code here
    //
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
