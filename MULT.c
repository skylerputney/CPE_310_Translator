#include "Instruction.h"

// will do later

void mult_reg_assm(void) {
	// Checking op code matches
	if (strcmp(OP_CODE, "MULT") != 0) {
		// Doesn't match - incorrect command
		state = WRONG_COMMAND;
		return;
	}
	
	/*
		Checking parameter types
	*/

	// First parameter is register
	if (PARAM1.type != REGISTER) {
		state = MISSING_REG;
		return;
	}

	// Second parameter is register
	if (PARAM2.type != REGISTER) {
		state = MISSING_REG;
		return;
	}

	/*
		Checking paramater values
	*/

	// Rs should be 31 or less
	if (PARAM1.value > 31) {
		state = INVALID_REG;
		return;
	}

	// Rt should be 31 or less
	if (PARAM2.value > 31) {
		state = INVALID_REG;
		return;
	}
	
	/*
		Assembling binary
	*/
	
	// Set opcode
	setBits_str(31, "000100");
	// Set funct
	setBits_str(5, "011000");
	// Set Rs
	setBits_num(25, PARAM1.value, 5);
	// Set Rt
	setBits_num(20, PARAM2.value, 5);
	// Set 15-6 as 0s
	setBits_str(15, "0000000000");

	// Signal encoding done
	state = COMPLETE_ENCODE;
}

void mult_reg_bin(void) {
	state = WRONG_COMMAND;
}

