
/*
    Purpose: Test all possible variations of all 3 register commands
    Params: None
    Return: None
*/
void testAll3RegCommands();

/*
    Purpose: Test all possible variations of a 3 register command
    Params: char* op: string representing command
    Return: None
*/
void testVariations3RegCommand(char* op);

/*
    Purpose: Test a variation of a 3 register command
    Params: char* op: string representing command
    Return: None
*/
void test3RegCommand(char* op);

/*
    Purpose: Assmble 3 register command using current values
    Params: char* dest_buffer: buffer to place command in, char* op -- string representing command
    Return: None
*/
void assemble3RegCommand(char* dest_buffer, char* op);

/*
    Purpose: Gets register in assembly format
    Params: char* dest_buffer: buffer to format, int reg_value: integer register value
    Return: None
*/
void getRegisterAssmbly(char* dest_buffer, int reg_value);

/*
    Purpose: Increments register 1, up to NUM_REGISTERS + 1, allowing for 1 value > allowed
    Params: None
    Return: None
*/
void incReg1();

/*
    Purpose: Increments register 2, up to NUM_REGISTERS + 1, allowing for 1 value > allowed
    Params: None
    Return: None
*/
void incReg2();

/*
    Purpose: Increments register 3, up to NUM_REGISTERS + 1, allowing for 1 value > allowed
    Params: None
    Return: None
*/
void incReg3();

/*
    Purpose: Reset all current values (except current command number) to default (0)
    Params: None
    Return: None
*/
void resetCurrs();

/*
    Purpose: Increment CURR_CMD to next
    Params: None
    Return: None
*/
void nextCommand();

/*
    Purpose: Reset current command number to default (0)
    Params: None
    Return: None
*/
void resetCommand();

void ensureReg1Valid();

void ensureReg2Valid();

void ensureReg3Valid();

void ensureRegistersValid();

void ensureImmValid();

/*
	Purpose: get binary machine code string of current encoded instruction
	Params: char* dest_buffer: Buffer to store binary representation in
	Return: none
*/
void getMachine(char* dest_buffer);

/*
	Purpose: get assembly string representation of currently decoded instruction
	Params: char* dest_buffer: Buffer to store assembly representation in
	Return: none
*/
void formatAssm(char* dest_buffer);

/*
	Purpose: Formats given parameter into assembly string representation
	Params: char* dest_buffer: Buffer to store assembly representation in, Param* param: parameter to convert
	Return: none
*/
void formatParam(char* dest_buffer, const struct Param* param);

