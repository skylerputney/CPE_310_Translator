#include "MIPS_Interpreter.h"
#include <assert.h>
#define NUM_REGISTERS 32
#define MAX_IMM_VALUE 0xFFFF
#define NUM_INSTRUCTIONS 18

int CURR_REG1 = 0;
int CURR_REG2 = 0;
int CURR_REG3 = 0;
int CURR_IMM = 0;


/*
    Represents the type of instruction based on parameters
*/
enum InstructionType {
    INSTR_3_REG, // ADD, AND, OR, SLT, SUB
    INSTR_2_REG, // DIV, MULT, 
    INSTR_1_REG, // MFHI, MFLO, 
    INSTR_2_REG_1_IMMD, // ANDI, BEQ, BNE, SLTI
    INSTR_1_REG_1_IMMD, // LUI, 
    INSTR_MEM // LW, SW
} typedef InstructionType;

/*
    Represents instruction's test parameter values
*/
typedef struct TestParams {
    int r1;
    int r2;
    int r3;
    int immd;
} TestParams;

/*
    Represents the instruction assembly OP, instruction type, and test parameter values
*/
typedef struct InstructionInfo {
    char op[6];
    InstructionType type;
    TestParams params;
} InstructionInfo;

// All supported commands with empty params initialized
InstructionInfo allInstructions[NUM_INSTRUCTIONS] = {
    {"ADD", INSTR_3_REG, {0, 0, 0, 0}},
    {"AND", INSTR_3_REG, {0, 0, 0, 0}},
    {"OR", INSTR_3_REG, {0, 0, 0, 0}},
    {"SLT", INSTR_3_REG, {0, 0, 0, 0}},
    {"SUB", INSTR_3_REG, {0, 0, 0, 0}},
    {"DIV", INSTR_2_REG, {0, 0, 0, 0}},
    {"MULT", INSTR_2_REG, {0, 0, 0, 0}},
    {"ANDI", INSTR_2_REG_1_IMMD, {0, 0, 0, 0}},
    {"BEQ", INSTR_2_REG_1_IMMD, {0, 0, 0, 0}},
    {"BNE", INSTR_2_REG_1_IMMD, {0, 0, 0, 0}},
    {"ORI", INSTR_2_REG_1_IMMD, {0, 0, 0, 0}},
    {"LUI", INSTR_1_REG_1_IMMD, {0, 0, 0, 0}},
    {"LW", INSTR_MEM, {0, 0, 0, 0}},
    {"SW", INSTR_MEM, {0, 0, 0, 0}},
};

void validateParams(InstructionInfo instr);
void testAllInstructions(void);
const char* num2reg(int num);
void assembleRightParamTypeInstruction(InstructionInfo instr, char* dest_buffer);
void testInstruction(InstructionInfo instr);

/*
    Purpose: Test all possible instruction variations
*/
void testAllInstructions(void){
    for(int instruction = 0; instruction < NUM_INSTRUCTIONS; instruction++){
        InstructionInfo curr_instr = allInstructions[instruction];
        for(int reg1 = -1; reg1 < NUM_REGISTERS + 1; reg1++){
            for(int reg2 = -1; reg2 < NUM_REGISTERS + 1; reg2++){
                for(int reg3 = -1; reg3 < NUM_REGISTERS + 1; reg3++){
                    for(int immd = -1; immd < MAX_IMM_VALUE + 2; immd++){
                        curr_instr.params.r1 = reg1;
                        curr_instr.params.r2 = reg2;
                        curr_instr.params.r3 = reg3;
                        curr_instr.params.immd = immd;
                    }
                }
            }
        }
    }
}

/*
    Purpose: Validate any instruction error states
*/
void validateParams(InstructionInfo instr){
    switch(instr.type){
        case INSTR_3_REG:
            if(instr.params.r1 < 0 || instr.params.r2 < 0 || instr.params.r3 < 0)
                assert(state == MISSING_PARAM);
            if(instr.params.r1 > NUM_REGISTERS - 1 || instr.params.r2 > NUM_REGISTERS - 1 || instr.params.r3 > NUM_REGISTERS - 1)
                assert(state == INVALID_REG);
            //if(instr.params.immd != -1)
                //assert(state == MISSING_REG)
            break;
        case INSTR_2_REG:
            if(instr.params.r3 != -1)
                assert(state == UNEXPECTED_PARAM);
            if(instr.params.r1 < 0 || instr.params.r2 < 0)
                assert(state == MISSING_PARAM);
            if(instr.params.r1 > NUM_REGISTERS - 1 || instr.params.r2 > NUM_REGISTERS - 1)
                assert(state == INVALID_REG);
            break;
        case INSTR_1_REG:
            if(instr.params.r2 != -1 || instr.params.r3 != -1)
                assert(state == UNEXPECTED_PARAM);
            if(instr.params.r1 < 0)
                assert(state == MISSING_PARAM);
            if(instr.params.r1 > NUM_REGISTERS - 1)
                assert(state == INVALID_REG);
            break;
        case INSTR_2_REG_1_IMMD:
            if(instr.params.r1 < 0 || instr.params.r2 < 0 || instr.params.immd < 0)
                assert(state == MISSING_PARAM);
            if(instr.params.r1 > NUM_REGISTERS - 1 || instr.params.r2 > NUM_REGISTERS - 1)
                assert(state == INVALID_REG);
            if(instr.params.r3 > MAX_IMM_VALUE)
                assert(state == INVALID_IMMED);
            break;
        case INSTR_1_REG_1_IMMD:
            if(instr.params.r1 < 0 || instr.params.immd < 0)
                assert(state == MISSING_PARAM);
            if(instr.params.r1 > NUM_REGISTERS - 1)
                assert(state == INVALID_REG);
            if(instr.params.r2 > MAX_IMM_VALUE)
                assert(state == INVALID_IMMED);
            break;
        case INSTR_MEM:
            if(instr.params.r1 < 0 || instr.params.immd < 0 || instr.params.r3 < 0)
                assert(state == MISSING_PARAM);
            if(instr.params.r1 > NUM_REGISTERS - 1 || instr.params.r3 > NUM_REGISTERS - 1)
                assert(state == INVALID_REG);
            if(instr.params.r2 > MAX_IMM_VALUE)
                assert(state == INVALID_IMMED);
    }
}


/*
    Purpose: Assemble a string representation of the given instruction with correct parameter types
*/
void assembleRightParamTypeInstruction(InstructionInfo instr, char* dest_buffer){
    switch(instr.type){
        case INSTR_3_REG:
            sprintf(dest_buffer, "%s %s, %s, %s", instr.op, num2reg(instr.params.r1), num2reg(instr.params.r2), num2reg(instr.params.r3));
            return;
        case INSTR_2_REG:
            sprintf(dest_buffer, "%s %s, %s", instr.op, num2reg(instr.params.r1), num2reg(instr.params.r2));
            return;
        case INSTR_1_REG:
            sprintf(dest_buffer, "%s %s", instr.op, num2reg(instr.params.r1));
            return;
        case INSTR_2_REG_1_IMMD:
            sprintf(dest_buffer, "%s %s, %s, #%d", instr.op, num2reg(instr.params.r1), num2reg(instr.params.r2), instr.params.immd);
            return;
        case INSTR_1_REG_1_IMMD:
            sprintf(dest_buffer, "%s %s, #%d", instr.op, num2reg(instr.params.r1), instr.params.immd);
            return;
        case INSTR_MEM:
            sprintf(dest_buffer, "%s %s, %d(%s)", instr.op, instr.params.r1, instr.params.immd, instr.params.r3);
            return;
    }
}

 /*
     Test the given instruction
 */
void testInstruction(InstructionInfo instr){
     if (state == NO_ERROR)
        encode();
}


/*
    Purpose: converts an integer register number to a string (register name)
    Params: uint32_t num - the register number
    Return: char* - register name as a string
*/
const char* num2reg(int num) {
    switch (num) {
        case 0: return "zero";  // Register 0 is $zero
        case 2: return "v0";    // Register 2 is $v0
        case 3: return "v1";    // Register 3 is $v1
        case 4: return "a0";    // Register 4 is $a0
        case 5: return "a1";    // Register 5 is $a1
        case 6: return "a2";    // Register 6 is $a2
        case 7: return "a3";    // Register 7 is $a3
        case 8: return "t0";    // Register 8 is $t0
        case 9: return "t1";    // Register 9 is $t1
        case 10: return "t2";   // Register 10 is $t2
        case 11: return "t3";   // Register 11 is $t3
        case 12: return "t4";   // Register 12 is $t4
        case 13: return "t5";   // Register 13 is $t5
        case 14: return "t6";   // Register 14 is $t6
        case 15: return "t7";   // Register 15 is $t7
        case 16: return "s0";   // Register 16 is $s0
        case 17: return "s1";   // Register 17 is $s1
        case 18: return "s2";   // Register 18 is $s2
        case 19: return "s3";   // Register 19 is $s3
        case 20: return "s4";   // Register 20 is $s4
        case 21: return "s5";   // Register 21 is $s5
        case 22: return "s6";   // Register 22 is $s6
        case 23: return "s7";   // Register 23 is $s7
        case 24: return "t8";   // Register 24 is $t8
        case 25: return "t9";   // Register 25 is $t9
        case 28: return "gp";   // Register 28 is $gp
        case 29: return "sp";   // Register 29 is $sp
        case 30: return "fp";   // Register 30 is $fp
        case 31: return "ra";   // Register 31 is $ra
        default: return "";   // Invalid register number
    }
}


