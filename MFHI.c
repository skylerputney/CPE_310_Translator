#include "Instruction.h"

void mfhi_reg_assm(void) {
	// Checking op code matches 
	if (strcmp(OP_CODE, "MFHI") != 0) {

		state = WRONG_COMMAND;
		return;
	}

	/*
		Checking the type of parameters
	*/

	// The only parameter should be a register (Rd)
	if (PARAM1.type != REGISTER) {
		state = MISSING_REG;
		return;
	}

	if (PARAM2.type != EMPTY) {
		state = UNEXPECTED_PARAM;
		return;
	}

	if (PARAM3.type != EMPTY) {
		state = UNEXPECTED_PARAM;
		return;
	}


	/*
		Checking the value of parameters
	*/

	// Rd should be 31 or less
	if (PARAM1.value > 31) {
		state = INVALID_REG;
		return;
	}

	/*
	Putting the binary together
	*/

	// Set the opcode
	setBits_str(31, "000000");
	// Set Rd
	setBits_num(15, PARAM1.value, 5);
	// Set the funct 
	setBits_str(5, "010000");
	// Set 25-16 as 0s 
	setBits_str(25, "0000000000");
	// Set 10-6 as 0s 
	setBits_str(10, "00000");

	// Signal encoding finished
	state = COMPLETE_ENCODE;
}

void mfhi_reg_bin(void) {
	// Check if the op code bits match
	// check_bits(start_bit, bit_string) returns 0 if the bit_string matches
	//  any x will be skipped
	// If the manual shows (0), then the value of that bit doesnt matter
	if (checkBits(31, "000000") != 0 || checkBits(5, "010000") != 0) {
		state = WRONG_COMMAND;
		return;
	}

	if (checkBits(25, "0000000000") != 0 || checkBits(10, "00000") != 0) {
		state = UNEXPECTED_PARAM;
		return;
	}

	// If the op code bits match, then the rest can be read as correctly

	/*
		Finding values in the binary
	*/

	// getBits(start_bit, width)
	uint32_t Rd = getBits(15, 5);

	/*
		Setting Instruction values
	*/

	setOp("MFHI");

	// setParam(param_num, param_type, param_value)
	setParam(1, REGISTER, Rd); //destination


	// Signal decoding finished
	state = COMPLETE_DECODE;
}


