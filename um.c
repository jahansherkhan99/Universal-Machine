/**************************************************************
 *                           um.c
 * 
 *     Assignment: Homework 6 - Universal Machine
 *     Authors: Archit Jain (ajain08), Jahansheer Khan (jkhan03)
 *     Date: Nov 21, 2021
 *
 *     This program contians main which executes the UM 
 *
 *     Purpose: This program takes a two arguments, the pathname 
 *     for a file (typically with a name like some program.um) 
 *     that contains machine instructions for your emulator to 
 *     execute. 
 *     
 *     Success Output: 
 *              The UM program runs correctly and executes all
 *              instructions 
 *        
 *     Failure output:
 *              A Hanson checked runtime exception is raised if 
 *              there is a problem executing the input file
 *                  
 **************************************************************/

 #include <stdlib.h>
 #include <assert.h>
 #include <stdio.h>
 #include "readfile.h"
 #include "execute_op.h"
 #include <sys/stat.h>

/*  Function: main
    Purpose: Call auxillary functions 
    Parameters: int argc, char *argv
    Returns: 0 if program ran succesfully, otherwise 1.
    Expectation: argc == 2
*/
int main(int argc, char *argv[]){
    
    if(argc != 2) {
        fprintf(stderr, "Error exiting failure\n");
        exit(EXIT_FAILURE); 
    }

    struct stat stats;
    
    if (stat(argv[1], &stats) == -1) {
        fprintf(stderr, "Error exiting failure\n");
        exit(EXIT_FAILURE);
    }
    if (stats.st_size % 4 != 0) {
        fprintf(stderr, "Error exiting faulure\n");
        exit(EXIT_FAILURE);
    }

    int proglength = stats.st_size / 4;
    
    /* Read in the file and store it in a sequence */
    UArray_T codewords = read_file(argv[1], proglength);
    
    /* Executes the instructions read in from the file and returns whether 
    program executed correctly */
    return execute(codewords);
}
 