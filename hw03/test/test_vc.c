#include "../src/libvc.h"

#include <211.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>

static void test_create_destroy(void)
{
    vc_destroy(vc_create());

    //test creating and destroying instances of NULL
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

static void test_helper_functions(void) {

    //functions to test:
    //vc_find_name()
    //vc_find_empty()
    //strdup_or_else()

    //CHECK_STRING( strdup_or_else("CS211"), "CS211")
}

static void test_vc_update(void) {

    //cases to test:
    //name is already present in vc
    //name is not already present in vc
    //if vc is full? maybe?

}

static void test_vc_lookup(void){

    //cases to test:
    //count for a candidate is 0
    //count for a candidate is nonzero
    //candidate not found in array
    //vc array is empty

}

static void test_vc_min_max(void) {

    //cases to test:
    //vc array is empty -- covered in test_2_candidates

    vote_count_t vc = vc_create();
    CHECK( vc != NULL );
    if (vc == NULL) {
        // Can't keep testing if vc is NULL
        return;
    }

    //candidates exist, but none have >0 votes
    size_t* cp_alice;
    size_t* cp_bob;
    size_t* cp_carol;
    
    cp_alice = vc_update(vc, "alice");
    cp_bob = vc_update(vc, "bob");
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

    cp_carol = vc_update(vc, "carol");
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

static void test_vc_total(void) {

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
    
    
}

static void test_vc_print(void) {

    //this function will likely just involve printing things,
    //so comment out any calls to it when not in use

    //cases to test:
    //0 candidates initialized
    //1 candidate
    //3 candidates
    //16 candidates
    //17 candidates

}

//
// You need more tests here.
//

int main(void)
{
    test_create_destroy();
    test_2_candidates();

    test_helper_functions();

    test_vc_min_max();
    test_vc_total();

    test_vc_lookup();
    test_vc_update();
}
