#include "Instruction.h"

void lw_immd_assm(void) {
	// Checking op code matches
	if (strcmp(OP_CODE, "LW") != 0) {
		// Doesn't match - incorrect command
		state = WRONG_COMMAND;
		return;
	}
	
	/*
		Checking parameter types
	*/
	
	// First parameter is register (Rt)
	if (PARAM1.type != REGISTER) {
		state = MISSING_REG;
		return;
	}

	// Second parameter is immediate (offset)
	if (PARAM2.type != IMMEDIATE) {
		state = INVALID_IMMED;
		return;
	}

	// Third parameter is register (base address, Rs)
	if (PARAM3.type != REGISTER) {
		state = MISSING_REG;
		return;
	}

	/*
		Checking parameter values
	*/

	// Register should be 31 or less
	if (PARAM1.value > 31) {
		state = INVALID_REG;
		return;
	}

	// The immediate value is limited to 16 bits, this is 0xFFFF
	if ( PARAM2.value > 0xFFFF) {
		state = INVALID_IMMED;
		return;
	}

	// Register should be 31 or less
	if ( PARAM3.value > 31) {
		state = INVALID_REG;
		return;
	}
	
	/*
		Assembling the binary
	*/

	// Set opcode
	setBits_str(31, "100011");
	// Set Rt
	setBits_num(20, PARAM1.value, 5);
	// Set Rs (base)
	setBits_num(25, PARAM3.value, 5);
	// Set immediate (offset)
	setBits_num(15, PARAM2.value, 16);

	// Signal encoding done
	state = COMPLETE_ENCODE;
}


void lw_immd_bin(void) {
	// Check op code bits match
	if (checkBits(31, "100011") != 0) {
		state = WRONG_COMMAND;
		return;
	}

	/*
		Obtain values from binary
	*/

	uint32_t Rs = getBits(25, 5);
	uint32_t Rt = getBits(20, 5);
	uint32_t imm16 = getBits(15, 16);

	/*
		Setting Instruction values
	*/

	setOp("LW");
	setParam(1, REGISTER, Rt);
	setParam(3, REGISTER, Rs);
	setParam(2, IMMEDIATE, imm16);

	// Signal decoding done
	state = COMPLETE_DECODE;

}
