/*
* Author: Ol' Jim
* Date: 06/13/2012
* ByteForge Systems
* MIPS-Translatron 3000
*/

// give this to the intern

#include "Instruction.h"

void slti_immd_assm(void) {
	// check if opcode matches
	if (strcmp(OP_CODE, "SLTI") != 0){
		state = WRONG_COMMAND;
		return;
	}

	/*
		Checking param types
	*/

	// param1 should be register (Rt)
	if (PARAM1.type != REGISTER) {
		state = MISSING_REG;
		return;
	}

	// param2 should be register (Rs)
	if (PARAM2.type != REGISTER) {
		state = MISSING_REG;
		return;
	}

	// param3 should be immediate
	if (PARAM3.type != IMMEDIATE) {
		state = INVALID_PARAM;
		return;
	}

	/*
		Checking the value of parameters
	*/

	// Rt must be <= 31
	if (PARAM1.value > 31) {
		state = INVALID_REG;
		return;
	}

	// Rs must be <= 31
	if (PARAM2.value > 31) {
		state = INVALID_REG;
		return;
	}

	// The immediate must be <= 0xFFFF
	if (PARAM3.value > 0xFFFF) {
		state = INVALID_IMMED;
		return;
	}

	/*
		Putting binary together
	*/

	// Setting opcode
	setBits_str(31, "001010");
	// Setting Rs
	setBits_num(25, PARAM2.value, 5);
	// Setting Rt
	setBits_num(20, PARAM1.value, 5);
	// Setting imm
	setBits_num(15, PARAM3.value, 16);

	// Set system state to complete encode
	state = COMPLETE_ENCODE;
}

void slti_immd_bin(void) {
	// Make sure opcode matches
	if(checkBits(31, "001010") != 0){
		state = WRONG_COMMAND;
		return;
	}

	/*
		Find values within binary
	*/

	// getBits(start_bit, width
	uint32_t Rs = getBits(25, 5);
	uint32_t Rt = getBits(20, 5);
	uint32_t imm16 = getBits(15, 16);

	/*
		Set instruction values
	*/

	setOp("SLTI");

	// setParam(param_num, param_type, param_value)
	setParam(1, REGISTER, Rt); // register to be set
	setParam(2, REGISTER, Rs); // register to be compared to imm
	setParam(3, IMMEDIATE, imm16); // imm used for comp

	// Set system state to complete decode
	state = COMPLETE_DECODE;
}


