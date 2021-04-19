#include "translate.h"

#include <stddef.h>
#include <stdlib.h>

//for the previous_range variable
#include <stdbool.h>

//for printing to standard error
#include <stdio.h>

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
    size_t result = 0;

    //start with the first character of src, at index 0
    size_t ind = 0;
    char letter = src[ind];

    //for help in a case like a-d-i, so as to tell the program
    //"intepret the - literally"
    bool previous_range = false;

    //for turning the pointers to these letters into their ints
    char backslash = *"\\";
    char dash = *"-";
    
    //iterate through each letter until reaching terminal 0
    while (letter != 0) {
        
        if (letter == backslash) {

            //check if the \ comes at the end of the src
            //interpret the "\" literally
            if (src[ind + 1] == 0) {          
                ++result;
                ++ind;
            }

            //check if the \ is the start of a range of chars
            else if (src[ind + 1] == dash && src[ind + 2] != 0) {
                ++result;
                ++ind;
            }           
             
            //else, interpret this and next char as 1 escape char
            else {
                ++result;
                ind += 2;
            }

            //the '\' character itself doesn't trigger a range
            previous_range = false;            
        }

        else if (letter == dash) {
            
            //check if we're at the start or end of the loop,
            //meaning we can't create a range and the '-' is literal
            if (ind == 0 || src[ind + 1] == 0) {
                ++result;
                ++ind;
                previous_range = false;
            }
            
            //check if the letter before was the end of a range, and
            //shouldn't be used in another range, like the a-d-i case
            else if (previous_range) {
                ++result;
                ++ind;
                previous_range = false;
            }
            
            else { //construct a range of chars

                char prev = src[ind - 1];
                char next = src[ind + 1];

                //case: if prev > next, range will be empty
                if (prev > next) {
                    ind += 2;
                    //"prev" was already counted, so undo that
                    --result;
                }
                else if (prev == next) {
                    //only one letter in range, and prev was already counted
                    ind += 2;
                }
                else {
                    //else, it's a normal range. as we already counted
                    //prev we only add the size of (next - prev) to result
                    size_t diff = next - prev;
                    result += diff;
                    ind += 2;
                }

                //since we've just checked a range, make sure the next char
                //doesn't get counted as a range
                previous_range = true;              
            }
            
        } else {
            //treat as a normal character
            ++result;
            ++ind;
            previous_range = false;
        }
           
        //go to next letter in src
        letter = src[ind];   
    }
    return result;
}

char* expand_charseq(const char* src)
{
    char* const dst = malloc(charseq_length(src) + 1);
    //if memory allocation fails:
    if (!dst) {
        return NULL;
    }

    //position in source and destination
    size_t s_ind = 0;
    size_t d_ind = 0;
    char letter = src[s_ind];
    
    //for turning the pointers to these letters into their ints
    char backslash = *"\\";
    char dash = *"-";
    //for use in the a-d-i type condition
    bool previous_range = false;

    while (letter != 0) {

        if (letter == backslash) {

            if (src[s_ind + 1] == 0) {
                //if we're at the end of src, backslash must stand for itself
                dst[d_ind] = *"\\";
                ++d_ind;
                ++s_ind;
            }
            else if (src[s_ind + 1] == dash && src[s_ind + 2] != 0) {
                //if the next two chars are a range, \ stands for itself
                dst[d_ind] = *"\\";
                ++d_ind;
                ++s_ind;
                
            } else {
                //interpret as escape character
                char next = src[s_ind + 1];
                char esc_char = interpret_escape(next);

                dst[d_ind] = esc_char;
                ++d_ind;
                s_ind += 2;                
            }
            previous_range = false;          
        }

        else if (letter == dash) {
            
            if (s_ind == 0 || src[s_ind + 1] == 0) {

                //if dash is at the start or end of string,
                //can't be a range, so interpret dash as itself
                dst[d_ind] = *"-";
                ++d_ind;
                ++s_ind;
                previous_range = false;
                
            } else if (previous_range) {

                //else if the previous character was the end of another
                //range, like a-d-i condition, interpret '-' as itself       
                dst[d_ind] = *"-";                
                ++d_ind;
                ++s_ind;
                previous_range = false;
                
            } else { //interpret as range

                char prev = src[s_ind - 1];
                char next = src[s_ind + 1];

                if (prev > next) {
                    //make no assignments; overwrite the "prev"
                    //since we already assigned "prev"
                    --d_ind;
                    s_ind += 2;
                    letter = src[s_ind];
                    
                } else if (prev == next) {
                    //prev was already put in dst, so make no assignment
                    s_ind += 2;
                }

                else {
                    //already assigned prev so start at prev + 1
                    for (char k = prev + 1; k <= next; ++k) {
                        dst[d_ind] = k;
                        ++d_ind;
                    }
                    //skip the "next" char
                    s_ind += 2;                   
                }
                //next thing that gets read (a-d-i case) isn't stored as range
                previous_range = true;                
            }
        }
        
        else {
            //treat as normal character
            dst[d_ind] = letter;
            ++d_ind;
            ++s_ind;
            previous_range = false;
        }
            
        letter = src[s_ind];
    }

    // reached the terminal 0
    dst[d_ind] = 0;
    return dst;
}

char translate_char(char c, const char* from, const char* to)
{   
    //index for iterating through "from" and "to"
    size_t ind = 0;

    //letters, or chars, at the current index of from and to
    char f_char = from[ind];
    char t_char = to[ind];

    //want to go until "from" and "to" terminate together in a 0 byte
    //assume "from" and "to" are the same size at this point
    while (!(f_char == 0 || t_char == 0)) {
              
        //check to see if char "c" is at the current position of from,
        //and if so, translate it to its equivalent in to
        if (f_char == c) {
            return t_char;
        }
        
        ++ind;
        f_char = from[ind];
        t_char = to[ind];
    }
    
    //if we've reached this point, we haven't found "c" in the "from"
    //sequence so we haven't translated it
    return c;
}

void translate(char* s, const char* from, const char* to)
{
    //for each char in the source s, perform translate_char to
    //see if items should be translated or not
    size_t ind = 0;
    char letter = s[ind];

    while (letter != 0) {

        //perform a translation on each char in the string "s"
        s[ind] = translate_char(s[ind], from, to);
        
        ++ind;
        letter = s[ind];
    }
}

