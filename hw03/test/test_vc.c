#include "../src/libvc.h"

#include <211.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>

static void test_create_destroy(void)
{
    vc_destroy(vc_create());

    //test creating and destroying instances of NULL
    vc_destroy(NULL);
}

static void test_2_candidates(void)
{
    // Skip this test if MAX_CANDIDATES isn't large enough.
    if (MAX_CANDIDATES < 2) {
        return;
    }

    size_t* cp;
    vote_count_t vc = vc_create();
    CHECK( vc != NULL );
    if (vc == NULL) {
        // Can't keep testing if vc is NULL
        return;
    }

    CHECK_POINTER( vc_max(vc), NULL );
    CHECK_POINTER( vc_min(vc), NULL );
    CHECK_SIZE( vc_lookup(vc, "alice"), 0 );
    CHECK_SIZE( vc_lookup(vc, "bob"), 0 );
    CHECK_SIZE( vc_total(vc), 0 );

    cp = vc_update(vc, "alice");
    CHECK( cp );
    *cp += 1;
 
    CHECK_STRING( vc_max(vc), "alice" );
    CHECK_STRING( vc_min(vc), "alice" );
    CHECK_SIZE( vc_lookup(vc, "alice"), 1 );
    CHECK_SIZE( vc_lookup(vc, "bob"), 0 );
    CHECK_SIZE( vc_total(vc), 1 );

    cp = vc_update(vc, "bob");
    CHECK( cp );
    *cp += 1;
    cp = vc_update(vc, "alice");
    CHECK( cp );
    *cp += 1;
    
    CHECK_STRING( vc_max(vc), "alice" );
    CHECK_STRING( vc_min(vc), "bob" );
    CHECK_SIZE( vc_lookup(vc, "alice"), 2 );
    CHECK_SIZE( vc_lookup(vc, "bob"), 1 );
    CHECK_SIZE( vc_total(vc), 3 );

    vc_destroy(vc);
}
  
//tests the vc_lookup() function.
static void test_vc_lookup(void){

    // Skip this test if MAX_CANDIDATES isn't large enough.
    if (MAX_CANDIDATES < 2) {
        return;
    }

    vote_count_t vc = vc_create();
    CHECK( vc != NULL );
    if (vc == NULL) {
        // Can't keep testing if vc is NULL
        return;
    }

    //cases to test:
    //vc array is empty
    CHECK_SIZE( vc_lookup(vc, "alice"), 0);
    
    //count for a candidate is 0
    size_t* cp_alice = vc_update(vc, "alice");
    CHECK_SIZE( vc_lookup(vc, "alice"), 0);
    size_t* cp_bob = vc_update(vc, "bob");
    CHECK_SIZE( vc_lookup(vc, "bob"), 0);
    
    //count for a candidate is nonzero
    *cp_alice += 2;
    CHECK_SIZE( vc_lookup(vc, "alice"), 2);
    *cp_bob += 123456789;
    CHECK_SIZE( vc_lookup(vc, "bob"), 123456789);

    //candidate not found in array
    CHECK_SIZE( vc_lookup(vc, "xavier"), 0);

    vc_destroy(vc);
}

//tests the vc_min() and vc_max() functions.
static void test_vc_min_max(void) {

    // Skip this test if MAX_CANDIDATES isn't large enough.
    if (MAX_CANDIDATES < 2) {
        return;
    }

    vote_count_t vc = vc_create();
    CHECK( vc != NULL );
    if (vc == NULL) {
        // Can't keep testing if vc is NULL
        return;
    }

    //cases to test:
    //vc array is empty -- covered in test_2_candidates
    
    //candidates exist, but none have >0 votes
   
    size_t* cp_alice = vc_update(vc, "alice");
    size_t* cp_bob = vc_update(vc, "bob");
    CHECK( cp_alice );
    CHECK( cp_bob );
    
    //max and min should return NULL if vc has no candidates
    //with more than 0 votes
    CHECK_POINTER( vc_max(vc), NULL);
    CHECK_POINTER( vc_min(vc), NULL);

    //several candidates, tie for min and max
    //max should return the earlier candidate;
    //min should return the later candidate
    *cp_alice += 1;
    *cp_bob += 1;
    CHECK_STRING( vc_max(vc), "alice");
    CHECK_STRING( vc_min(vc), "bob");

    size_t* cp_carol = vc_update(vc, "carol");
    *cp_carol += 1;
    CHECK_STRING( vc_max(vc), "alice");
    CHECK_STRING( vc_min(vc), "carol");

    //several candidates, with a single min/max earner
    *cp_carol += 2;
    *cp_bob += 1;
    CHECK_STRING( vc_max(vc), "carol");
    CHECK_STRING( vc_min(vc), "alice");
    
    vc_destroy(vc);
}

//tests the vc_update() and vc_total() functions.
static void test_vc_update_total(void) {

    // Skip this test if MAX_CANDIDATES isn't large enough.
    if (MAX_CANDIDATES < 2) {
        return;
    }
    
    vote_count_t vc = vc_create();
    CHECK( vc != NULL );
    if (vc == NULL) {
        // Can't keep testing if vc is NULL
        return;
    }

    //when vc array is empty: covered in test 2 candidates
    
    //several candidates but no votes earned
    size_t* cp_alice = vc_update(vc, "alice");
    size_t* cp_bob = vc_update(vc, "bob");
    CHECK_SIZE( vc_total(vc), 0);

    //1 earner only
    *cp_alice += 1;
    CHECK_SIZE( vc_total(vc), 1);

    //3 earners
    size_t* cp_carol = vc_update(vc, "carol");
    *cp_bob += 2;
    *cp_carol += 55;
    CHECK_SIZE( vc_total(vc), 58);

    //cases to test for vc_update():
    //name is not already present in vc
    CHECK_SIZE( *vc_update(vc, "doug"), 0);
    
    //name is already present in vc
    CHECK_SIZE( *vc_update(vc, "alice"), 1);
    CHECK_SIZE( *vc_update(vc, "bob"), 2);
    CHECK_SIZE( *vc_update(vc, "carol"), 55);
    
    //if vc is full
    vc_update(vc, "e");
    vc_update(vc, "f");
    vc_update(vc, "g");
    vc_update(vc, "h");
    vc_update(vc, "i");
    vc_update(vc, "j");
    vc_update(vc, "k");
    vc_update(vc, "l");
    vc_update(vc, "m");
    vc_update(vc, "n");
    vc_update(vc, "o");
    vc_update(vc, "p");

    //this'll be the 17th one; for M_C = 16 this exceeds the max
    CHECK_POINTER( vc_update(vc, "q"), NULL);
    CHECK_SIZE( vc_total(vc), 58);
    //vc_print(vc);
    
    vc_destroy(vc);
}

//tests the table printing function for a vc array.
//this function involves  printing things,
//so comment out any calls to it when not in use
static void test_vc_print(void) {

    // Skip this test if MAX_CANDIDATES isn't large enough.
    if (MAX_CANDIDATES < 2) {
        return;
    }
    
    vote_count_t vc = vc_create();
    CHECK( vc != NULL );
    if (vc == NULL) {
        // Can't keep testing if vc is NULL
        return;
    }

    //when no candidates are initialized
    vc_print(vc);
    printf("\n");
    
    //one candidate but no votes earned
    size_t* cp_alice = vc_update(vc, "alice");
    vc_print(vc);
    printf("\n");
    
    //after two candidates have earned votes
    size_t* cp_bob = vc_update(vc, "bob");
    *cp_alice += 5;
    *cp_bob += 2;
    
    vc_print(vc);
    printf("\n");

    //three candidates, one hasn't earned votes
    vc_update(vc, "carol");
    vc_print(vc);
    printf("\n");

    //if vc is full
    vc_update(vc, "d");
    vc_update(vc, "e");
    vc_update(vc, "f");
    vc_update(vc, "g");
    vc_update(vc, "h");
    vc_update(vc, "i");
    vc_update(vc, "j");
    vc_update(vc, "k");
    vc_update(vc, "l");
    vc_update(vc, "m");
    vc_update(vc, "n");
    vc_update(vc, "o");
    vc_update(vc, "p");

    //for M_C = 16, this "q" exceeds the max
    vc_update(vc, "q");
    vc_print(vc);

    vc_destroy(vc);
}


int main(void)
{
    test_create_destroy();
    test_2_candidates();

    test_vc_min_max();
    test_vc_update_total();

    test_vc_lookup();
    //test_vc_print();
}
