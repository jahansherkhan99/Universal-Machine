/**************************************************************
 *                       execute_op.h
 *
 *     Assignment: Homework 6 - Universal Machine
 *     Authors: Archit Jain (ajain08), Jahansheer Khan (jkhan03)
 *     Date: Nov 21, 2021
 *
 *     Interface for our execute_op
 *
 *     Purpose: This program takes in a sequence of instructions
 * 				and then executes each instruction one by one.
 * 				If an instruction requires memory access then it
 * 				calls functions from the seg_mem file.
 *
 *
 *     Success Output:
 *              Each instruction is run successfully and there
 * 				are no errors in execution
 *
 *     Failure output:
 *              A Hanson checked runtime exception is raised if
 *              there is a problem executing any instruction
 *
 **************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <bitpack.h>
#include <stdint.h>
#include "readfile.h"
#include "seq.h"
#include "uarray.h"

#ifndef EXECUTE_OP_H
#define EXECUTE_OP_H

typedef struct Um *Um;

int execute(UArray_T seg_0);

void add_registers(Um vals, uint32_t instruction);
void freeMem(Um vals);

void loadvalue(Um vals, uint32_t instruction);
void conditional_move(Um vals);
void seg_load(Um values);
void seg_store(Um values);
void add(Um vals);
void multiply(Um vals);
void divide(Um vals);
void nand(Um vals);
void halt(Um vals);
void output(Um vals);
void input(Um vals);
void seg_map(Um vals);
void seg_unmap(Um vals);
void loadprogram(Um vals);

#endif
/* EXECUTE_OP_H */