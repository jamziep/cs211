#include "libvc.h"

#include <211.h>

#include <stdio.h>

#define OOM_MESSAGE     "%s: out of memory\n"
#define DROP_MESSAGE    "%s: vote dropped: %s\n"
#define FINAL_MESSAGE   "%s: %zu vote(s) dropped\n"


// count.c takes candidate names from stdinput
// also drops names once it has more names than can handle




int main(int argc, char* argv[])
{
    
    vote_count_t c_pool = vc_create();

    if(!c_pool){
        fprintf(stderr, OOM_MESSAGE, "./count");
        return 1;
    }
    
    size_t num_cands = 0;
    size_t dropped_votes = 0;

     while(1)
     {             // this is running in an infinite loop for now
        const char* name = read_line();
        if(!name)
        {
            break;
        }

        if(num_cands < MAX_CANDIDATES || vc_lookup(c_pool, name) != 0)
        {
            //will count votes as long as MAX has not been exceeded.
            // if max is reached then checks if candidate is already in the pool. If not, drops the candidate

            
            size_t* count_ptr = vc_update(c_pool, name);
        
            if(vc_lookup(c_pool, name) == 0)
            {             // if the candidate has yet to recieve any of the votes, they are a new candidate and num_cands needs to be incremented
                ++num_cands;   
            }
        
            *count_ptr +=  1;   // add one to the count for curr candidate
        
            printf("count : %ld\n", *count_ptr);        // these prints were just for debugging
            printf("number of cands: %ld\n", num_cands);            
        }else
        {
            printf(DROP_MESSAGE, "./count", name);
            ++dropped_votes;     
        }

        // free(&name);
     }
     
     
     vc_print(c_pool);
     vc_destroy(c_pool);
     
    //comment here
}

