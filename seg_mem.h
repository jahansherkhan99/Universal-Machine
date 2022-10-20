/**************************************************************
 *                     seg_mem.h
 *
 *     Assignment: Homework 6 - Universal Machine
 *     Authors: Archit Jain (ajain08), Jahansheer Khan (jkhan03)
 *     Date: Nov 21, 2021
 *
 *     interface for our seg_mem
 *
 *     Purpose: Used to access, map, unmap memory segments through
 * 				the program, and the functions in this file are
 * 				called by the execute_op file
 *
 *     Success Output:
 *              Memory is successfully allocated and deallocated
 * 				through the program and this is done efficiently
 * 				with mapped and unmapped segments
 *
 *     Failure output:
 *              A Hanson checked runtime exception is raised if
 *              there is a problem with accessing any memory
 * 				segment
 *
 **************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <bitpack.h>
#include <stdint.h>
#include "execute_op.h"
#include "seq.h"
#include "uarray.h"

#ifndef SEG_MEM_H
#define SEG_MEM_H

typedef struct MemSeg_T *MemSeg_T;

MemSeg_T seg_new();
void seg_free(MemSeg_T memory_total);
MemSeg_T seg_initial(MemSeg_T memory_total, UArray_T codewords);
uint32_t segment_load(MemSeg_T memory_total, uint32_t regB, uint32_t regC);
void segment_store(MemSeg_T memory_total, uint32_t regA, uint32_t regB,
                      uint32_t regC);
uint32_t map_segment(MemSeg_T memory_total, int length);
void unmap_segment(MemSeg_T memory_total, uint32_t id);
UArray_T get_segment(MemSeg_T memory_total, int id);
void set_seg_0(MemSeg_T memory_total, UArray_T segment);

#endif
/* SEG_MEM_H */