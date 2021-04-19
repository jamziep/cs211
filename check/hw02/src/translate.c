#include "translate.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// Helper function for interpreting escapes. Given the character that
// follows a backslash, returns the character that the combination
// should be replaced with. Recognizes the seven standard C escapes, and
// maps every other character to itself.
char interpret_escape(char c)
{
    switch (c) {
    case 'a': return '\a';
    case 'b': return '\b';
    case 'f': return '\f';
    case 'n': return '\n';
    case 'r': return '\r';
    case 't': return '\t';
    case 'v': return '\v';
    default:  return c;
    }
}

size_t charseq_length(const char* src)
{
    size_t i  = 0;
    size_t result = 0;
    size_t prev_range = false;

    while(src[i] != 0){
        if(src[i]  ==  '-'){    // case for ranges
            if(i == 0 || src[i+1] == 0 || prev_range){      // if - is the first char, before the null terminator, or if the start of the range was already used
                ++i;
                result++;
            }else if((src[i+1] >= src[i-1])) {      // if last is greater than first, go ahead with range
                result = result + (src[i+1] - src[i-1]);
                i = i + 2;
                prev_range = true;
            }else{
                --result;
                i = i + 2;
            }
            
        }else if(src[i] == '\\'){     // case for escape sequence
            if((src[i+1] == 0) || src[i+1] == '-'){    // for cases where the following is the null terminator or a start of range (treat'\\' like normal char)
                result++;
                ++i;
     
            }else{                // for cases where'\\' is a true escape
                result++;
                i = i + 2;
                       
            }
            prev_range = false;   
            
        }else{             // case for normal chars
            ++i;
            result++;
            prev_range = false;
        }
        
    }
    return result;
}

char* expand_charseq(const char* src)
{
    char* const dst = malloc(charseq_length(src) + 1);
    if (!dst) {
        return NULL;
    }
    //  void *home = dst;
    size_t i = 0;   // index for the source string
    size_t j = 0;  // index for new string
    size_t prev_range = false;

    while(src[i] != 0){
        if(src[i] == '-'){      // cases for range
            if(i == 0 || src[i+1] == 0 || prev_range){
                dst[j] = src[i];
                ++i;
                ++j;
                    
            }else if((src[i-1] <= src[i+1])){
                for(size_t start = src[i-1] + 1; start <= src[i+1]; start++){
                    dst[j] = start;
                    ++j;  
                }
                prev_range = true;
                i = i + 2;
                
            }else{
                --j;
                i = i + 2;
            }

        }else if(src[i] == '\\'){   // case for interrupt
            if(src[i+1] == 0 || src[i+1] == '-'){   // '\\' is before null terminator or start of range...
                dst[j] = '\\';
                ++i;
                ++j;         
                
            }else{  // otherwise treat as proper null terminator
            dst[j] = interpret_escape(src[i+1]);
            ++j;
            i = i + 2;
            }
            prev_range = false;

        }else{            // all other valid chars
            dst[j] = src[i];
            ++i;
            ++j;
            prev_range = false;
        }
    }

    dst[j] = 0;
    return dst;
 
    //  free(dst); // this is wrong this needs to deallocate before return is called
}

// The starter code for expand_charseq above is written in
// *array-indexing style*, whereas the commented-out expand_charseq
// below uses *pointer arithmetic style*. If you prefer pointer
// arithmetic to array indexing, you may comment out the expand_charseq
// above and uncomment the one right here:
/*
char* expand_charseq(const char* src)
{
    char* result = malloc(charseq_length(src) + 1);
    if (!result) {
        return NULL;
    }

    char* dst    = result;

    //
    // TODO: Your code goes here
    //

    *dst = 0;

    return result;
}
*/

char translate_char(char c, const char* from, const char* to)
{
    size_t ii = 0;
    while(from[ii] != 0){
        if(from[ii] == c){
            return to[ii];
        }
        ++ii;   
    }

    return c;
}

void translate(char* s, const char* from, const char* to)
{
    size_t jj = 0;
    while(s[jj] != 0){
        s[jj] = translate_char(s[jj], from, to);
        ++jj;
    }
}

