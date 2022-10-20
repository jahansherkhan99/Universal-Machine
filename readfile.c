/**************************************************************
 *                     readfile.c
 * 
 *     Assignment: Homework 6 - Universal Machine
 *     Authors: Archit Jain (ajain08), Jahansheer Khan (jkhan03)
 *     Date: Nov 21, 2021
 *
 *     implementation for our readfile.h 
 *
 *     Purpose: Used to read in input from a um file passed in 
 *              as a parameter and then store that input and 
 *              pass it as a parameter to another file for 
 *              execution
 *     
 *     Success Output: 
 *              There is valid input with an EOF character at the
 *              end of the file 
 *        
 *     Failure output:
 *              A Hanson checked runtime exception is raised if 
 *              there is a problem reading in the input file
 *                  
 **************************************************************/

#include <bitpack.h>
#include <stdint.h>
#include "readfile.h"
#include "execute_op.h"

static const unsigned BYTESIZE = 8; 

/*  Function: read_file
    Purpose: this function reads in a file and stores each instruction in 
    a Uarray of uint32_t
    Parameters: a filename and the total length of the program 
    Returns: a Uarray with all the 32-bit instructions 
    Expectation: a valid filename entered by the user 
*/
UArray_T read_file(char* file_name, int length)
{
    /* check for a valid filename input */ 
    assert(file_name != NULL);
    
    /* declare File pointer to hold opened file */ 
    FILE *file;
        
    /* try opening the file */
    file = fopen(file_name, "rb");
    assert(file != NULL);

    /* Create a UArray to hold the segment 0 with instructions */
    UArray_T seg_0 = UArray_new(length, sizeof(uint32_t));

    /* input taken in one byte at a time, and one instruction is four bytes */
    int byte = 0;  
    byte = fgetc(file);

    /* iterator to set words in the Uarray */
    int word_index = 0;

    /* get a byte, store it, and continue this process until there is an 
    EOF read in as input */
    while(byte != EOF) {  
        /* initialise word to be initially 0 */
        uint32_t word = 0;
        
        /* for loop used ot get one instruction and store it in word */
        for (int i = 3 * BYTESIZE; i >= 0; i = i - BYTESIZE) {
            word = (uint32_t) Bitpack_newu((uint64_t) word, BYTESIZE, i, 
                                            (uint64_t) byte);  
            byte = fgetc(file);
        }
        
        /* word stored in sequence */
        uint32_t *temp = (uint32_t *) UArray_at(seg_0, word_index);
        *temp = word;  
        assert(temp != NULL);

        /* increment index of word */
        word_index++;
    }    

    return seg_0;
}