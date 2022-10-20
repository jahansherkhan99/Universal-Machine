/**************************************************************
 *                     readfile.h
 * 
 *     Assignment: Homework 6 - Universal Machine
 *     Authors: Archit Jain (ajain08), Jahansheer Khan (jkhan03)
 *     Date: Nov 21, 2021
 *
 *     interface for our readfile 
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

#include <stdio.h>
#include <stdlib.h>
#include <assert.h> 
#include "uarray.h"

/* File defination READFILE_H */
#ifndef READFILE_H
#define READFILE_H

UArray_T read_file(char* file_name, int size);

#endif
/* READFILE_H */