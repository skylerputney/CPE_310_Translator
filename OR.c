#include "Instruction.h"

void or_reg_assm(void) {
	// Checking that opcode is correct
	if (strcmp(OP_CODE, "OR") != 0) {
		state = WRONG_COMMAND;
		return;
	}

	/*
		Checking the type of parameters
	*/

	// param1 should be a register (rd)
	if (PARAM1.type != REGISTER) {
		state = MISSING_REG;
		return;
	}

	// param2 should be a register (rs)
	if (PARAM2.type != REGISTER) {
		state = MISSING_REG;
		return;
	}

	// param3 should be a register (rt)
	if (PARAM3.type != REGISTER) {
		state = MISSING_REG;
		return;
	}

	/*
		Checking param values
	*/

	//  rd must be <= 31
	if (PARAM1.value > 31) {
		state = INVALID_REG;
		return;
	}

	// rs must be <= 31
	if (PARAM2.value > 31) {
		state = INVALID_REG;
		return;
	}

	// rt must be <= 31
	if (PARAM3.value > 31) {
		state = INVALID_REG;
		return;
	}

	/*
		Putting binary together
	*/

	// Setting opcode
	setBits_num(31, 0, 6);

	// Setting funct
	setBits_str(5, "100101");

	// Setting rd
	setBits_num(15, PARAM1.value, 5);

	// Setting rs
	setBits_num(25, PARAM2.value, 5);

	// Setting rt
	setBits_num(20, PARAM3.value, 5);

	// Set system state to complete encode
	state = COMPLETE_ENCODE;
}

void or_reg_bin(void) {
	// checking if opcode and funct bits match
	if (checkBits(31, "000000") != 0 || checkBits(5, "100101") != 0) {
		state = WRONG_COMMAND;
		return;
	}

	/*
		Finding values in binary
	*/
	uint32_t Rd = getBits(15, 5);
	uint32_t Rs = getBits(25, 5);
	uint32_t Rt = getBits(20, 5);

	/*
		Setting instruction values
	*/
	setOp("OR"); // op value
	setParam(1, REGISTER, Rd); // destination reg
	setParam(2, REGISTER, Rs); // source reg
	setParam(3, REGISTER, Rt); // second source reg

	// set system state to complete decode
	state = COMPLETE_DECODE;
}


