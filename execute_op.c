/**************************************************************
 *                       execute_op.c
 *
 *     Assignment: Homework 6 - Universal Machine
 *     Authors: Archit Jain (ajain08), Jahansheer Khan (jkhan03)
 *     Date: Nov 21, 2021
 *
 *     Implementation for out execute_op.h
 *
 *     Purpose: This program takes in a sequence of instructions
 * 				and then executes each instruction one by one.
 * 				If an instruction requires memory access then it
 * 				calls functions from the seg_mem file.
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

#include <bitpack.h>
#include <stdint.h>
#include "execute_op.h"
#include "seg_mem.h"

/* constant values for the register number and opcode instructions */
enum registerNum { REGA = 0, REGB, REGC };
enum opcode { CMOV = 0, SLOAD, STORE, ADD, MUL, DIV,
    NAND, HALT, SEGMAP, UNMAP, OUT, IN, LOADP, LV };

/* The maximum and minimum values for character during input and output */
static const int CHAR_MAX = 255;
static const int CHAR_MIN = 0;

/* this struct holds four variables
    1. An uint32_t array of the eight registers
    2. An uint32_t array of the register numbers a, b, and c
    3. A struct MemSeg_T holding an implementation of the memory
    4. The segment 0 with instructions of the file
    5. A program counter that loops through instructions
*/
struct Um {
    uint32_t regs[8];
    uint32_t regNum[3];
    MemSeg_T memory_total;
    UArray_T seg_0;
    int prog_ctr;
};

/*  Function: execute
    Purpose: Executes all the opcodes in the program
    Parameters: UArray_T of seg_0 (the program itself)
    Returns:  Integer indicating succesful completion
*/
int execute(UArray_T seg_0)
{
    /* allocate space for the struct for runtime */
    Um values = malloc(sizeof(struct Um));
    assert(values != NULL);

    /* set initial values of the struct */
    values->seg_0 = seg_0;
    values->memory_total = seg_new();
    values->memory_total = seg_initial(values->memory_total, values->seg_0);
    for(int i = 0; i < 8; i++) {
        values->regs[i] = 0;
    }
    for(int i = 0; i < 3; i++) {
        values->regNum[i] = 0;
    }

    /* traverse through segment 0 and execute each instruction based on the
    opcode */
    for (values->prog_ctr = 0; values->prog_ctr < UArray_length(values->seg_0);
                                        values->prog_ctr++) {
        /* get instruction */
        uint32_t instruction = * (uint32_t *) UArray_at(values->seg_0,
                                                        values->prog_ctr);
        /* get opcode from instruction */
        uint32_t op = Bitpack_getu(instruction, 4, 28);

        /* execute instruction based on opcode */
        if(op == CMOV) {
            add_registers(values, instruction);
            conditional_move(values);
        }
        else if (op == SLOAD) {
            add_registers(values, instruction);
            seg_load(values);
        }
        else if (op == STORE) {
            add_registers(values, instruction);
            seg_store(values);
        }
        else if (op == ADD) {
            add_registers(values, instruction);
            add(values);
        }
        else if (op == MUL) {
            add_registers(values, instruction);
            multiply(values);
        }
        else if (op == DIV) {
            add_registers(values, instruction);
            divide(values);
        }
        else if (op == NAND) {
            add_registers(values, instruction);
            nand(values);
        }
        else if (op == HALT) {
            halt(values);
            return 0;
        }
        else if (op == SEGMAP) {
            add_registers(values, instruction);
            seg_map(values);
        }
        else if (op == UNMAP) {
            add_registers(values, instruction);
            seg_unmap(values);
        }
        else if (op == OUT) {
            add_registers(values, instruction);
            output(values);
        }
        else if (op == IN) {
            add_registers(values, instruction);
            input(values);
        }
        else if (op == LOADP) {
            add_registers(values, instruction);
            loadprogram(values);
        }
        else if (op == LV) {
            loadvalue(values, instruction);
        }
        else {
            assert(0);
        }
    }
    freeMem(values);
    return 1;
}

/*  Function: add_registers
    Purpose: Retrives register values from the seg_0 of the program
    Parameters: Struct of registers, the codeword of instructions
    Returns: N/A
*/
void add_registers(Um vals, uint32_t instruction)
{
    /* check for valid input */
    assert(vals != NULL);

    /* get register numbers */
    vals->regNum[REGA] = Bitpack_getu(instruction, 3, 6);
    vals->regNum[REGB] = Bitpack_getu(instruction, 3, 3);
    vals->regNum[REGC] = Bitpack_getu(instruction, 3, 0);
}

void freeMem(Um vals)
{
    /* check for valid input */
    assert(vals != NULL);

    /* free memory_total and the struct */
    if(vals->memory_total != NULL) {
        seg_free(vals->memory_total);
    }
    free(vals);
}

/*  Function: conditional_move
    Purpose: Struct of registers
    Parameters: Struct of registers, the codeword of instructions
    Returns:  N/A
*/
void conditional_move(Um vals)
{
    /* check for valid input */
    assert(vals != NULL);

    /* if REGC = 0, then move value from B to A */
    if (vals->regs[vals->regNum[REGC]] != 0) {
        vals->regs[vals->regNum[REGA]] = vals->regs[vals->regNum[REGB]];
    }
}

/*  Function: seg_load
    Purpose: Loads value from m[rB][rC] into register A
    Parameters: Struct of registers, the codeword of instructions
    Returns:  N/A
*/
void seg_load(Um vals)
{
    /* check for valid input */
    assert(vals != NULL);

    /* load a segment and return value to REGA */
    vals->regs[vals->regNum[REGA]] = segment_load(vals->memory_total,
                                            vals->regs[vals->regNum[REGB]],
                                            vals->regs[vals->regNum[REGC]]);
}
/*  Function: seg_store
    Purpose: Loads value of register C into m[rA][rB] into register A
    Parameters: Struct of registers, the codeword of instructions
    Returns:  N/A
*/
void seg_store(Um vals)
{
    /* check for valid input */
    assert(vals != NULL);

    /* Loads value of register C into m[rA][rB] */
    segment_store(vals->memory_total,
                vals->regs[vals->regNum[REGA]],
                vals->regs[vals->regNum[REGB]],
                vals->regs[vals->regNum[REGC]]);

}

/*  Function: add
    Purpose: Adds values in register B and register C and loads into reg A
    Parameters: Struct of registers
    Returns:  N/A
*/
void add(Um vals)
{
    /* check for valid input */
    assert(vals != NULL);

    /* adds values and stores it into regA */
    vals->regs[vals->regNum[REGA]] = vals->regs[vals->regNum[REGB]] +
    vals->regs[vals->regNum[REGC]];
}

/*  Function: multiply
    Purpose: Multiplies values in register B and register C and loads into
    reg A
    Parameters: Struct of registers
    Returns:  N/A
*/
void multiply(Um vals)
{
    /* check for valid input */
    assert(vals != NULL);

    /* multiply values and stores it into regA */
    vals->regs[vals->regNum[REGA]] = vals->regs[vals->regNum[REGB]] *
    vals->regs[vals->regNum[REGC]];
}

/*  Function: divide
    Purpose: Divides values in register B and register C and loads into reg A
    Parameters: Struct of registers
    Returns:  N/A
*/
void divide(Um vals)
{
    /* check for valid input */
    assert(vals != NULL);

    /* divide values and stores it into regA */
    vals->regs[vals->regNum[REGA]] = vals->regs[vals->regNum[REGB]]
    / vals->regs[vals->regNum[REGC]];
}

/*  Function: nand
    Purpose: Bitwise reverse "and" of values in register B and C, loading
    the result into register A
    Parameters: Struct of registers
    Returns:  N/A
*/
void nand(Um vals)
{
    /* check for valid input */
    assert(vals != NULL);

    /* nand values and RegB and RegC and stores it into regA */
    vals->regs[vals->regNum[REGA]] = ~ (vals->regs[vals->regNum[REGB]]
                                        & vals->regs[vals->regNum[REGC]]);
}

/*  Function: halt
    Purpose: Halts the program and frees memory
    Parameters: UArray of seg_0 and struct of registers
    Returns:  N/A
*/
void halt(Um vals)
{
    /* check for valid input */
    assert(vals != NULL);
    freeMem(vals);
}

/*  Function: output
    Purpose: To output an ASCII character to stdout.
    Parameters: struct of registers
    Returns:  N/A
    Expectatoins: A value from 0 to 255
*/

void output(Um vals)
{
    /* check for valid input */
    assert(vals != NULL);

    /* get character and check with range, print */
    int rc = vals->regs[vals->regNum[REGC]];
    assert(rc <= CHAR_MAX);
    assert(rc >= CHAR_MIN);
    putchar(rc);
}

/*  Function: input
    Purpose: Receives a character from standard input.
    Parameters: struct of registers
    Returns:  N/A
    Expectatoins: A value from 0 to 255
*/
void input(Um vals)
{
    /* check for valid input */
    assert(vals != NULL);

    /* get character and check with range, store it in REGC */
    int rc = getchar();
    assert(rc <= CHAR_MAX);
    assert(rc >= CHAR_MIN || rc == EOF);

    /* if character is EOF store it as the EOF value */
    if(rc != EOF) {
        vals->regs[vals->regNum[REGC]] = rc;
    }
    else {
        vals->regs[vals->regNum[REGC]] = ~0;
    }
}

/*  Function: seg_map
    Purpose: Loads the index of the newly mapped memory segment into
             register B.
    Parameters: struct of registers
    Returns:  N/A
*/
void seg_map(Um vals)
{
    /* check for valid input */
    assert(vals != NULL);

    /* map segment and store it onto register at register num B */
    vals->regs[vals->regNum[REGB]] = map_segment(vals->memory_total,
                                          vals->regs[vals->regNum[REGC]]);
}

/*  Function: unmap_segment
    Purpose: Frees memory from mapped ID's, which is retrieved from RegC
    Parameters: A MemSeg_T to access memory from
    Returns: N/A
    Expectation: the struct must not be NULL
*/
void seg_unmap(Um vals)
{
    /* check for valid input */
    assert(vals != NULL);

    /* unmap segment */
    unmap_segment(vals->memory_total, vals->regs[vals->regNum[REGC]]);
}

/*  Function: loadprogram
    Purpose: Replaces the program being run with memory location ID from
    register B
    Parameters: A struct of registers, and the main program being run
    Returns: N/A
    Expectation: the struct must not be NULL
*/
void loadprogram(Um vals)
{
    /* check for valid input */
    assert(vals != NULL);

    if (vals->regs[vals->regNum[REGB]] != 0) {

        UArray_T segment = get_segment(vals->memory_total,
                                vals->regs[vals->regNum[REGB]]);
        UArray_T new_segment = UArray_new(UArray_length(segment),
        sizeof(uint32_t));
        for (int i = 0; i < UArray_length(segment); i++) {
            uint32_t *ptr = UArray_at(new_segment, i);
            *ptr = *(uint32_t *)UArray_at(segment, i);
        }
        set_seg_0(vals->memory_total, new_segment);
        vals->seg_0 = new_segment;
    }

    /* set program counter to register c value */
    vals->prog_ctr = vals->regs[vals->regNum[REGC]] - 1;
}

/*  Function: load_value
    Purpose: Retrieves a 25 bit integer and stores it in Register A
    Parameters: Struct of registers, the codeword of instructions
    Returns:  N/A
*/
void loadvalue(Um vals, uint32_t instruction)
{
    /* check for valid input */
    assert(vals != NULL);

    vals->regNum[REGA] = Bitpack_getu(instruction, 3, 25);
    uint32_t value = Bitpack_getu(instruction, 25, 0);
    vals->regs[vals->regNum[REGA]] = value;
}