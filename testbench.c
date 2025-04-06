#include "MIPS_Interpreter.h"
NUM_REGISTERS = 32;
MAX_IMM_VALUE = 0xFFFF;
NUM_COMMANDS = 18;

CURR_REG1 = 0;
CURR_REG2 = 0;
CURR_REG3 = 0;
CURR_IMM = 0;

CURR_CMD = 0;

// Represent commands
enum command {
    ADD,
    ADDI,
    AND,
    ANDI,
    BEQ,
    BNE,
    DIV,
    LUI,
    LW,
    MFHI,
    MFLO,
    MULT,
    OR,
    ORI,
    SLT,
    SLTI,
    SUB,
    SW
} typedef command;

/*
    Purpose: Test all possible variations of all 3 register commands
    Params: None
    Return: None
*/
void testAll3RegCommands(){
    testVariations3RegCommands("ADD");
    testVariations3RegCommands("SUB");
    testVariations3RegCommand("AND");
    testVariations3RegCommand("OR");
    testVariations3RegCommand("SLT");
}

/*
    Purpose: Test all possible variations of a 3 register command
    Params: char* op: string representing command
    Return: None
*/
void testVariations3RegCommand(char* op){
    for(int reg1 = 0; reg1 < 33; reg1++) {
        for(int reg2 = 0; reg2 < 33; reg2++) {
            for(int reg3 = 0; reg3 < 33; reg3++) {
                test3RegCommand(op);
                incReg3();
            }
            incReg2();
        }
        incReg1();
    }
}

/*
    Purpose: Test a variation of a 3 register command
    Params: char* op: string representing command
    Return: None
*/
void test3RegCommand(char* op){
        // Assemble instruction
        char instruction[20] = {0};
        assemble3RegCommand(instruction, op);

        // Parse instruction
		parseAssem(instruction);

        ensureRegistersInvalid();

		// Encode if no error
		if (state == NO_ERROR) {
			encode();
		}

        // Get machine code string
        char machineCode[33] = {0};
        getMachine(machineCode);

        // Parse binary
        parseBin(machineCode);

        // Decode if no error
        if (state == NO_ERROR) {
			decode();
		}

        //format assembly
        char resultAssm[20] = {0};
        formatAssm(resultAssm);
        
        assert(strcmp(instruction, resultAssm) == 0);
}


/*
    Purpose: Assmble 3 register command using current values
    Params: char* dest_buffer: buffer to place command in, char* op -- string representing command
    Return: None
*/
void assemble3RegCommand(char* dest_buffer, char* op){
    sprintf(dest_buffer, "%s, %d, %d, %d", op, CURR_REG1, CURR_REG2, CURR_REG3);
}

/*
    Purpose: Gets register in assembly format
    Params: char* dest_buffer: buffer to format, int reg_value: integer register value
    Return: None
*/
void getRegisterAssmbly(char* dest_buffer, int reg_value){
    sprintf(dest_buffer, "$t%d", reg_value)
}


/*
    Purpose: Increments register 1, up to NUM_REGISTERS + 1, allowing for 1 value > allowed
    Params: None
    Return: None
*/
void incReg1() {
    CURR_REG1 = (CURR_REG1 + 1) % (NUM_REGISTERS + 1);
}

/*
    Purpose: Increments register 2, up to NUM_REGISTERS + 1, allowing for 1 value > allowed
    Params: None
    Return: None
*/
void incReg2() {
    CURR_REG2 = (CURR_REG2 + 1) % (NUM_REGISTERS + 1);
}

/*
    Purpose: Increments register 3, up to NUM_REGISTERS + 1, allowing for 1 value > allowed
    Params: None
    Return: None
*/
void incReg3() {
    CURR_REG3 = (CURR_REG3 + 1) % (NUM_REGISTERS + 1);
}

/*
    Purpose: Reset all current values (except current command number) to default (0)
    Params: None
    Return: None
*/
void resetCurrs(){
    CURR_REG1 = 0;
    CURR_REG2 = 0;
    CURR_REG3 = 0;
    CURR_IMM = 0;
}

/*
    Purpose: Increment CURR_CMD to next
    Params: None
    Return: None
*/
void nextCommand(){
    CURR_CMD += 1;
}

/*
    Purpose: Reset current command number to default (0)
    Params: None
    Return: None
*/
void resetCommand(){
    CURR_CMD = 0;
}

void ensureReg1Valid(){
    if (CURR_REG1 > 31)
        assert(state == INVALID_REG);
}

void ensureReg2Valid(){
    if (CURR_REG2 > 31)
        assert(state == INVALID_REG);
}

void ensureReg3Valid(){
    if (CURR_REG3 > 31)
        assert(state == INVALID_REG);
}

void ensureRegistersValid(){
    ensureReg1Invalid();
    ensureReg2Invalid();
    ensureReg3Invalid();
}

void ensureImmValid(){
    if (CURR_IMM > 0xFFFF)
        assert(state == INVALID_IMMED);
}

/*
	Purpose: get binary machine code string of current encoded instruction
	Params: char* dest_buffer: Buffer to store binary representation in
	Return: none
*/
void getMachine(char* dest_buffer){
    int pos = 0;

    for (int i = 31; i >= 0; i--) {
        dest_buffer[pos++] = ((BIN32 >> i) & 1) ? '1' : '0';

        if (i % 4 == 0 && i != 0) {
            dest_buffer[pos++] = ' ';
        }
    }

    dest_buffer[pos] = '\0';  // Null-terminate the string
}

/*
	Purpose: get assembly string representation of currently decoded instruction
	Params: char* dest_buffer: Buffer to store assembly representation in
	Return: none
*/
void formatAssm(char* dest_buffer) {
    char temp[32];
    int pos = 0;

    // op code
    pos += sprintf(dest_buffer + pos, "%s", assm_instruct.op);

    // param 1
    if (PARAM1.type != EMPTY) {
        pos += sprintf(dest_buffer + pos, " ");
        formatParam(temp, &PARAM1);
        pos += sprintf(dest_buffer + pos, "%s", temp);
    }

    // param 2
    if (PARAM2.type != EMPTY) {
        pos += sprintf(dest_buffer + pos, ", ");
        formatParam(temp, &PARAM2);
        pos += sprintf(dest_buffer + pos, "%s", temp);
    }

    // param 3
    if (PARAM3.type != EMPTY) {
        if (PARAM3.type == REGISTER && 
           (strcmp(OP_CODE, "LW") == 0 || strcmp(OP_CODE, "SW") == 0)) {
            pos += sprintf(dest_buffer + pos, "(");
            formatParam(temp, &PARAM3);
            pos += sprintf(dest_buffer + pos, "%s)", temp);
        } else {
            pos += sprintf(dest_buffer + pos, ", ");
            formatParam(temp, &PARAM3);
            pos += sprintf(dest_buffer + pos, "%s", temp);
        }
    }

    // param 4
    if (PARAM4.type != EMPTY) {
        pos += sprintf(dest_buffer + pos, ", ");
        formatParam(temp, &PARAM4);
        pos += sprintf(dest_buffer + pos, "%s", temp);
    }

    dest_buffer[pos] = '\0'; // Ensure null termination
}

/*
	Purpose: Formats given parameter into assembly string representation
	Params: char* dest_buffer: Buffer to store assembly representation in, Param* param: parameter to convert
	Return: none
*/
void formatParam(char* dest_buffer, const struct Param* param) {
    uint32_t temp;

    switch (param->type) {
    case EMPTY:
        sprintf(dest_buffer, "<>");
        break;

    case REGISTER:
        temp = param->value;
        if (temp == 0) {
            sprintf(dest_buffer, "$zero");
        }
        else if (temp == 2 || temp == 3) {
            sprintf(dest_buffer, "$v%d", temp - 2);
        }
        else if (temp >= 4 && temp <= 7) {
            sprintf(dest_buffer, "$a%d", temp - 4);
        }
        else if (temp >= 8 && temp <= 15) {
            sprintf(dest_buffer, "$t%d", temp - 8);
        }
        else if (temp >= 16 && temp <= 23) {
            sprintf(dest_buffer, "$s%d", temp - 16);
        }
        else if (temp == 24 || temp == 25) {
            sprintf(dest_buffer, "$t%d", temp - 16);
        }
        else if (temp == 28) {
            sprintf(dest_buffer, "$gp");
        }
        else if (temp == 29) {
            sprintf(dest_buffer, "$sp");
        }
        else if (temp == 30) {
            sprintf(dest_buffer, "$fp");
        }
        else if (temp == 31) {
            sprintf(dest_buffer, "$ra");
        }
        else {
            sprintf(dest_buffer, "$r%d", temp);  // fallback for unknown register
        }
        break;

    case IMMEDIATE:
        sprintf(dest_buffer, "#0x%X", param->value);
        break;

    default:
        sprintf(dest_buffer, "<unknown: %d, %d>", param->type, param->value);
        break;
    }
}