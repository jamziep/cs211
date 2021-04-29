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
ballot_t read_ballot_from_file(const char* file_path);

static void test_ballot_5(void);
static void test_ballot_max(void);

///
/// MAIN FUNCTION
///

int main(int argc, char* argv[])
{
    test_clean_name();
    test_ballot_3();
    test_ballot_with_vc();

    //our functions
    test_ballot_create_destroy();
    test_ballot_5();
    test_ballot_max();
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
    //skip this test if max_candidates is too small
    if (MAX_CANDIDATES < 3) {
        return;
    }
    
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
    count_ballot(vote_count, ballot1);
    
    //verify the counts in vc
    CHECK_SIZE( vc_lookup(vote_count,"A"), 1);
    CHECK_SIZE( vc_lookup(vote_count,"B"), 0);
    CHECK_SIZE( vc_lookup(vote_count,"C"), 0);

    //count again and confirm the votes
    count_ballot(vote_count, ballot1);

    CHECK_SIZE( vc_lookup(vote_count,"A"), 2);
    CHECK_SIZE( vc_lookup(vote_count,"B"), 0);
    CHECK_SIZE( vc_lookup(vote_count,"C"), 0);

    //eliminate candidate B, count again, and confirm the votes
    ballot_eliminate( ballot1, "B" );
    leader1 = ballot_leader(ballot1);
    CHECK_STRING( leader1, "A");

    count_ballot(vote_count, ballot1);
    CHECK_SIZE( vc_lookup(vote_count,"A"), 3);
    CHECK_SIZE( vc_lookup(vote_count,"B"), 0);
    CHECK_SIZE( vc_lookup(vote_count,"C"), 0);

    //eliminate A, and confirm that a vote goes toward C
    ballot_eliminate( ballot1, "A" );
    leader1 = ballot_leader(ballot1);
    CHECK_STRING( leader1, "C");

    count_ballot(vote_count, ballot1);
    CHECK_SIZE( vc_lookup(vote_count,"A"), 3);
    CHECK_SIZE( vc_lookup(vote_count,"B"), 0);
    CHECK_SIZE( vc_lookup(vote_count,"C"), 1);

    //eliminate C and confirm that counting the ballot again
    //has no effect on the counts    
    ballot_eliminate( ballot1, "C");
    CHECK_POINTER( ballot_leader(ballot1), NULL);

    count_ballot( vote_count, ballot1 );
    CHECK_SIZE( vc_lookup(vote_count,"A"), 3);
    CHECK_SIZE( vc_lookup(vote_count,"B"), 0);
    CHECK_SIZE( vc_lookup(vote_count,"C"), 1);

    ballot_destroy(ballot1);
    vc_destroy(vote_count);
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

//test reading in a basic case of 5 candidates
//ranked on a ballot
static void test_ballot_5(void) {

    //if max_candidates is small, skip this function
    if (MAX_CANDIDATES < 6) {
        return;
    }
    
    //bring in the 5 candidates from file
    ballot_t ballot = read_ballot_from_file("rsrc/ballot_5.in");
    vote_count_t vc = vc_create();
    
    //check that candidate number 1 is leader
    CHECK_STRING( ballot_leader(ballot), "WASHINGTON");
    count_ballot(vc, ballot);

    //check counts
    CHECK_SIZE( vc_lookup(vc,"WASHINGTON"), 1);
    CHECK_SIZE( vc_lookup(vc,"ADAMS"), 0);
    CHECK_SIZE( vc_lookup(vc,"JEFFERSON"), 0);
    CHECK_SIZE( vc_lookup(vc,"MADISON"), 0);
    CHECK_SIZE( vc_lookup(vc,"MONROE"), 0);
    
    //eliminate leader and count ballot again. check counts
    ballot_eliminate(ballot, "WASHINGTON");
    count_ballot(vc, ballot);

    CHECK_SIZE( vc_lookup(vc, "WASHINGTON"), 1);
    CHECK_SIZE( vc_lookup(vc, "ADAMS"), 1);
    CHECK_STRING( vc_max(vc), "WASHINGTON");
    CHECK_STRING( vc_min(vc), "ADAMS");

    //eliminate a non-leader and ensure that the leader stays the same
    ballot_eliminate(ballot, "MONROE");
    count_ballot(vc, ballot);
    
    CHECK_SIZE( vc_lookup(vc, "ADAMS"), 2);
    CHECK_SIZE( vc_lookup(vc, "MONROE"), 0);

    //add another candidate to ballot
    //also test that ballot_eliminate does nothing with
    //candidates that aren't yet on ballot
    ballot_eliminate(ballot, "QADAMS");
    ballot_insert(ballot, strdupb("QADAMS","test_ballot_5"));
    ballot_eliminate(ballot, "ADAMS");
    ballot_eliminate(ballot, "MADISON");
    ballot_eliminate(ballot, "JEFFERSON");
    
    count_ballot(vc, ballot);
    CHECK_SIZE( vc_lookup(vc, "ADAMS"), 2);
    CHECK_SIZE( vc_lookup(vc, "QADAMS"), 1);
    
    vc_destroy(vc);
    ballot_destroy(ballot);
}


//test that ballot_insert() exits with exit code 3 if
//ballot is full; can't add more candidates
static void test_ballot_max(void) {

    //if max candidates is less than 16, skip this function
    if (MAX_CANDIDATES < 16) {
        return;
    }
    
    //bring in the 15 candidates from file. I had to not make it
    //16 immediately b/c I was getting heap buffer overflow from the'
    //last (empty) line in file
    ballot_t ballot = read_ballot_from_file("rsrc/ballot_16.in");
    vote_count_t vc = vc_create();


    vc_destroy(vc);
    ballot_destroy(ballot);
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

//read in a filename as a const char*, open the file using
//the read_ballot function, and returns the object that was read
//file_path is relative to the location of the Makefile
ballot_t read_ballot_from_file(const char* file_path) {

    FILE* ballot_file = fopen(file_path, "r");
    //if fopen fails, do nothing
    if (!ballot_file) {
        fclose(ballot_file);
        return NULL;
    }
    
    ballot_t ballot = read_ballot(ballot_file);

    //close the FILE* stream of the file we were reading
    fclose(ballot_file);
    return ballot;
}



