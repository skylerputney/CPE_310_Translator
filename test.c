#define NUM_REGISTERS 32
#define MAX_IMM_VALUE 0xFFFF
#define NUM_COMMANDS 18

CURR_REG1 = 0;
CURR_REG2 = 0;
CURR_REG3 = 0;
CURR_IMM = 0;

CURR_CMD = 0;

/*
    Represents the type of instruction based on parameters
*/
enum InstructionType {
    INSTR_3_REG, // ADD, AND, OR, SLT, SUB
    INSTR_2_REG, // DIV, MULT, 
    INSTR_1_REG, // MFHI, MFLO, 
    INSTR_2_REG_1_IMMD, // ANDI, BEQ, BNE, SLTI
    INSTR_1_REG_1_IMMD, // LUI, 
    INSTR_MEM, // LW, SW
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


