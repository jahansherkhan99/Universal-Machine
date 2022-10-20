/**************************************************************
 *                     seg_mem.c
 *
 *     Assignment: Homework 6 - Universal Machine
 *     Authors: Archit Jain (ajain08), Jahansheer Khan (jkhan03)
 *     Date: Nov 21, 2021
 *
 *     implementation for our seg_mem.h
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

#include "uarray.h"
#include "seg_mem.h"

/* inital size of sequence */
static const int NEWSEQ = 0;

/* this struct holds two variables
    1. A sequence of UArrays holding memory
    2. A sequence of unmapped IDs to be used by the program
*/
struct MemSeg_T
{
    Seq_T heap;
    Seq_T unmapped_IDs;
};

/*  Function: seg_new
    Purpose: this function creates a new struct of the memory segment
    with a MemSeg_T of memory and MemSeg_T of unmapped_IDs
    Parameters: none
    Returns: an allocated MemSeg_T
    Expectation: none
*/
MemSeg_T seg_new()
{
    /* malloc space for segment */
    MemSeg_T segment = malloc(sizeof(struct MemSeg_T));
    assert(segment != NULL);

    /* initialise sequences of MemSeg_T */
    segment->heap = Seq_new(NEWSEQ);
    segment->unmapped_IDs = Seq_new(NEWSEQ);

    /* return MemSeg_T */
    return segment;
}

/*  Function: seg_free
    Purpose: frees all the memory associated with the MemSeg_T struct
    Parameters: A MemSeg_T to be freed
    Returns: none
    Expectation: the struct must not be NULL
*/
void seg_free(MemSeg_T memory_total)
{
    /* check for memory_total not being NULL */
    assert(memory_total != NULL);

    if(memory_total->heap != NULL) {
        /* free memory associated with the memory segment */
        int heap_length = Seq_length(memory_total->heap);
        for(int i = 0; i < heap_length; i++) {
            UArray_T temp =  (UArray_T) Seq_get(memory_total->heap, i);
            if(temp != NULL) {
                UArray_free(&temp);
            }
        }
        Seq_free(&(memory_total->heap));
    }

    if(memory_total->unmapped_IDs != NULL) {
        /* free memory associated with the unmapped IDs */
        int ids_length = Seq_length(memory_total->unmapped_IDs);
        for(int i = 0; i < ids_length; i++) {
            uint32_t *temp =  (uint32_t *)
                                    Seq_get(memory_total->unmapped_IDs, i);
            if (temp != NULL) {
                free(temp);
            }
        }
        Seq_free(&(memory_total->unmapped_IDs));
    }

    /* free the struct MemSeg_T */
    free(memory_total);
}

/*  Function: seg_initial
    Purpose: creates segment 0 of the program and adds the set of instructions
    to it
    Parameters: A MemSeg_T to add Segment 0 to and the set of instructions
    Returns: A MemSeg_T with the values
    Expectation: the struct and instruction UArray must not be NULL
*/
MemSeg_T seg_initial(MemSeg_T memory_total, UArray_T codewords)
{
    /* check for valid input */
    assert(codewords != NULL);
    assert(memory_total != NULL);

    /* add segment 0 with instructions */
    Seq_addhi(memory_total->heap, codewords);

    /* return the MemSeg_T */
    return memory_total;
}

/*  Function: segment_load
    Purpose: Value of at m[regB][regC] is extracted and returned
    Parameters: A MemSeg_T to access memory from, two registers
    Returns: A MemSeg_T with the values
    Expectation: the struct must not be NULL
*/
uint32_t segment_load(MemSeg_T memory_total, uint32_t regB, uint32_t regC)
{
     /* check for valid input */
    assert(memory_total != NULL);

    /* get value at register */
    UArray_T temp = (UArray_T) Seq_get(memory_total->heap, regB);
    assert(temp != NULL);

    /* set value in memory */
    uint32_t val = * (uint32_t *) UArray_at(temp, regC);

    /* return value */
    return val;
}

/*  Function: segment_store
    Purpose: Stores the value of register C into memory location m[regA][regB]
    Parameters: A MemSeg_T to access memory from, three registers
    Returns: N/A
    Expectation: the struct and instruction UArray must not be NULL
*/
void segment_store(MemSeg_T memory_total, uint32_t regA, uint32_t regB,
                      uint32_t regC)
{
     /* check for valid input */
    assert(memory_total != NULL);

    /* get value at register A */
    UArray_T temp = (UArray_T) Seq_get(memory_total->heap, regA);

    /* store value onto register B */
    uint32_t *ptr = (uint32_t *)UArray_at(temp, regB);
    *ptr = regC;
}

/*  Function: map_segment
    Purpose: Allocates memory of size requested by the user
    Parameters: A MemSeg_T to access memory from, size of requested memory
    Returns: uint32_t
    Expectation: the struct must not be NULL, length must be greater than 0
*/
uint32_t map_segment(MemSeg_T memory_total, int length)
{
    /* check for valid input */
    assert(Seq_length(memory_total->heap) != 0);

    /* create new UArray to map segemnt */
    UArray_T temp = UArray_new(length, sizeof(uint32_t));

    /* set each element in the Uarray to 0 */
    for (int i = 0; i < UArray_length(temp); i++) {
        uint32_t *ptr = UArray_at(temp, i);
        *ptr = 0;
    }

    /* Add the UArray onto memory based on whether there is space.
    If there are no unmapped ids add to end of sequence, otherwise,
    add to the first index of unmapped IDs */
    if (Seq_length(memory_total->unmapped_IDs) == 0)  {
        Seq_addhi(memory_total->heap, temp);
        return Seq_length(memory_total->heap) - 1;
    }
    else {
        uint32_t *index = (uint32_t *) Seq_remhi (memory_total->unmapped_IDs);
        uint32_t ind = *index;
        free(index);
        Seq_put(memory_total->heap, ind, temp);
        return ind;
    }
}

/*  Function: unmap_segment
    Purpose: Frees memory from mapped ID's, and keeps a track of which ids have
    been freed
    Parameters: A MemSeg_T to access memory from, id of block to be freed
    Returns: N/A
    Expectation: the struct must not be NULL, id must be greater than 0
*/
void unmap_segment(MemSeg_T memory_total, uint32_t id)
{
    /* Get segment at index and free it */
    UArray_T temp = Seq_get(memory_total->heap, id);
    if(temp != NULL) {
        UArray_free(&temp);
    }

    /* malloc space for the id to add it onto unmapped ids */
    uint32_t *addid = malloc(sizeof(uint32_t));
    assert(addid != NULL);
    *addid = id;

    /* add the id to unmapped ids and add NULL to the corresponding place
    in Memory */
    Seq_addhi(memory_total->unmapped_IDs, addid);
    Seq_put(memory_total->heap, id, NULL);
}

/*  Function: get_segment
    Purpose: gets a memory segment and returns to the client
    Parameters: A MemSeg_T to access memory from, id of block to be returned
    Returns: A UArray_T with memory
    Expectation: the struct must not be NULL, id must be greater than 0
*/
UArray_T get_segment(MemSeg_T memory_total, int id)
{
    /* get map segment at the id passed and test for it not being NULL */
    UArray_T temp = Seq_get(memory_total->heap, id);
    assert(temp != NULL);

    /* return it the segment */
    return temp;
}

/*  Function: set_seg_0
    Purpose: sets segment 0 and replaces the old set of codewords
    Parameters: A MemSeg_T to access memory from, id of block to be returned
    Returns: N/A
    Expectation: the struct & UArrray must not be NULL
*/
void set_seg_0(MemSeg_T memory_total, UArray_T segment)
{
    assert (memory_total != NULL);

    UArray_T seg_0 = (UArray_T)Seq_get(memory_total->heap, 0);
    UArray_free(&seg_0);
    Seq_put(memory_total->heap, 0, segment);
}