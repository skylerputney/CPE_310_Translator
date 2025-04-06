#include "MIPS_Interpreter.h"
#define NUM_REGISTERS 32
#define MAX_IMM_VALUE 0xFFFF
#define NUM_INSTRUCTIONS 18

int CURR_REG1 = 0;
int CURR_REG2 = 0;
int CURR_REG3 = 0;
int CURR_IMM = 0;

int CURR_INSTR = 0;

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

void validateParams(void);

void validateParams(InstructionInfo instr){
    switch(instr.type){
        case INSTR_3_REG:
            if(instr.params.r1 < 0 || instr.params.r2 < 0 || instr.params.r3 < 0)
                assert(state == MISSING_PARAM);
            if(instr.params.r1 > 31 || instr.params.r2 > 31 || instr.params.r3 > 31)
                assert(state == INVALID_REG);
        case INSTR_2_REG:
            if(instr.params.r3 != -1)
                assert(state == UNEXPECTED_PARAM);
            if(instr.params.r1 < 0 || instr.params.r2 < 0)
                assert(state == MISSING_PARAM);
            if(instr.params.r1 > 31 || instr.params.r2 > 31)
                assert(state == INVALID_REG);
    }
}


