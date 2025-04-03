/*
* Author: Ol' Jim
* Date: 06/13/2012
* ByteForge Systems
* MIPS-Translatron 3000
*/


#include "Instruction.h"

void lui_immd_assm(void) {
	// Check if the OPCODE matches LUI
	if (strcmp(OP_CODE, "LUI") != 0) {
		// If the opcode is not LUI then the state is wrong command
		state = WRONG_COMMAND;
		return;
	}

	/*
		Parameter checking
	*/

	// First param should be register
	if (PARAM1.type != REGISTER) {
		state = MISSING_REG;
		return;
	}
	
	// Second param should be immediate
	if (PARAM2.type != IMMEDIATE) {
		state = INVALID_PARAM;
		return;
	}

	/*
		Checking param values
	*/

	// Rt should be less than or equal to 31
	if (PARAM1.value > 31) {
		state = INVALID_REG;
		return;
	}

	// imm should be less than or equal to 0xFFFF
	if (PARAM2.value > 0xFFFF) {
		state = INVALID_IMMED;
		return;
	}

	/*
		Putting binary together
	*/

	// Setting opcode
	setBits_str(31, "001111");

	// Setting dont care bits
	setBits_str(25, "00000");

	// Setting rt value
	setBits_num(20, PARAM1.value, 5);

	// Setting imm value
	setBits_num(15, PARAM2.value, 16);

	// Set system state to complete encode
	state = COMPLETE_ENCODE;
}

void lui_immd_bin(void) {
	// Check if opcode bits are correct
	if (checkBits(31, "001111") != 0) {
		state = WRONG_COMMAND;
		return;
	}

	/*
		Finding values in the machine code
	*/

	uint32_t Rt = getBits(20, 5);
	uint32_t imm16 = getBits(15, 16);

	/*
		Setting instruction values
	*/

	setOp("LUI");
	setParam(1, REGISTER, Rt); // register param
	setParam(2, IMMEDIATE, imm16); // immediate param

	// Set system state to complete decode
	state = COMPLETE_DECODE;
}