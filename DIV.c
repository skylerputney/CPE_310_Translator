/*
* Author: Ol' Jim
* Date: 06/13/2012
* ByteForge Systems
* MIPS-Translatron 3000
*/

#include "Instruction.h"

void div_reg_assm(void) {
	// Checking op code matches
	if (strcmp(OP_CODE, "DIV") != 0) {
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
		Checking parameter values
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
	setBits_str(5, "011010");
	// Set Rs
	setBits_num(25, PARAM1.value, 5);
	// Set Rt
	setBits_num(20, PARAM2.value, 5);
	// Set 15-6 as 0s
	setBits_num(15, 0, 10);
	
	// Signal encoding done
	state = COMPLETE_ENCODE;
}

void div_reg_bin(void) {
	// Check opcode bits match
	if (checkBits(31, "000000") != 0 || checkBits(5, "011010") != 0) {
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

	setOp("DIV");
	setParam(1, REGISTER, Rs);
	setParam(2, REGISTER, Rt);

	// Signal decoding done
	state = COMPLETE_DECODE;
}


