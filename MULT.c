#include "Instruction.h"


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

	// First parameter is register (Rs)
	if (PARAM1.type != REGISTER) {
		state = MISSING_REG;
		return;
	}

	// Second parameter is register (Rt)
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
	setBits_str(31, "000000");

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
	// Check op code bits match and dead bits are 0
	if (checkBits(31, "000000") != 0 || checkBits(5, "011000") != 0 || checkBits(15, "0000000000") != 0) {
		state = WRONG_COMMAND;
		return;
	}

	/*
		Obtain values from binary
	*/

	uint32_t Rs = getBits(25, 5);
	uint32_t Rt = getBits(20, 5);

	/*
		Set instruction values
	*/
	setOp("MULT");
	setParam(1, REGISTER, Rs);
	setParam(2, REGISTER, Rt);

	// Signal decoding done
	state = COMPLETE_DECODE;
}

