#include "Instruction.h"

void andi_immd_assm(void) {

	if (strcmp(OP_CODE, "ANDI") != 0) {
		// Wrong command if op code doesn't match
		state = WRONG_COMMAND;
		return;
	}

	/*
		Checking the type of parameters
	*/

	// First parameter is a register (Rt)
	if (PARAM1.type != REGISTER) {
		state = MISSING_REG;
		return;
	}

	// Second parameter is a register (Rs)
	if (PARAM2.type != REGISTER) {
		state = MISSING_REG;
		return;
	}

	// Third parameter is an immediate
	if (PARAM3.type != IMMEDIATE) {
		state = INVALID_PARAM;
		return;
	}

	/*
		Checking parameter values
	*/

	//Rt should be 31 or less
	if (PARAM1.value > 31) {
		state = INVALID_REG;
		return;
	}

	// Rs should be 31 or less
	if (PARAM2.value > 31) {
		state = INVALID_REG;
		return;
	}

	// Immediate value limited to 16 bits, 0xFFFF
	if (PARAM3.value > 0xFFFF) {
		state = INVALID_IMMED;
		return;
	}

	/*
		Assembling the binary
	*/

	// Set opcode
	setBits_str(31, "001100");
	// Set Rt
	setBits_num(20, PARAM1.value, 5);
	// Set Rs
	setBits_num(25, PARAM2.value, 5);
	// Set immediate
	setBits_num(15, PARAM3.value, 16);

	// Tell system encoding done
	state = COMPLETE_ENCODE;
}

void andi_immd_bin(void) {
	if (checkBits(31, "001100") != 0 ) {
		// Wrong command if opcode doesn't match
		state = WRONG_COMMAND;
		return;
	}

	/*
		Obtain values from binary
	*/

	// getBits(start_bit, width)
	uint32_t Rs = getBits(25, 5);
	uint32_t Rt = getBits(20, 5);
	uint32_t imm16 = getBits(15, 16);

	/*
		Set instruction values
	*/

	setOp("ANDI");

	// setParam(param_num, param_type, param_value)
	setParam(1, REGISTER, Rt); 
	setParam(2, REGISTER, Rs); 
	setParam(3, IMMEDIATE, imm16);
	
	// Tell system decoding done
	state = COMPLETE_DECODE;
}



