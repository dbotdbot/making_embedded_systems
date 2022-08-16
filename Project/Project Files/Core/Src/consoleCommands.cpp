// ConsoleCommands.c
// This is where you add commands:
//		1. Add a protoype
//			static eCommandResult_T ConsoleCommandVer(const char buffer[]);
//		2. Add the command to mConsoleCommandTable
//		    {"ver", &ConsoleCommandVer, HELP("Get the version string")},
//		3. Implement the function, using ConsoleReceiveParam<Type> to get the parameters from the buffer.


#include <string.h>
#include "consoleCommands.h"
#include "console.h"
#include "consoleIo.h"
#include "version.h"
#include "global.h"
#include "main.h"

#define IGNORE_UNUSED_VARIABLE(x)     if ( &x == &x ) {}

static eCommandResult_T ConsoleCommandComment(const char buffer[]);
static eCommandResult_T ConsoleCommandVer(const char buffer[]);
static eCommandResult_T ConsoleCommandHelp(const char buffer[]);
static eCommandResult_T ConsoleCommandParamExampleInt16(const char buffer[]);
static eCommandResult_T ConsoleCommandParamExampleHexUint16(const char buffer[]);
static eCommandResult_T ConsoleCommandLedRed(const char buffer[]);
static eCommandResult_T ConsoleCommandLedGreen(const char buffer[]);
static eCommandResult_T ConsoleCommandLedBlue(const char buffer[]);
static eCommandResult_T ConsoleCommandGetSetpoint(const char buffer[]);
static eCommandResult_T ConsoleCommandSetSetpoint(const char buffer[]);
static eCommandResult_T ConsoleCommandSetAngle(const char buffer[]);
static eCommandResult_T ConsoleCommandGetCurrentPos(const char buffer[]);
static eCommandResult_T ConsoleCommandSetMode(const char buffer[]);
static eCommandResult_T ConsoleCommandSetKp(const char buffer[]);
static eCommandResult_T ConsoleCommandSetKi(const char buffer[]);
static eCommandResult_T ConsoleCommandSetKd(const char buffer[]);

static const sConsoleCommandTable_T mConsoleCommandTable[] =
{
    {";", &ConsoleCommandComment, HELP("Comment! You do need a space after the semicolon. ")},
    {"help", &ConsoleCommandHelp, HELP("Lists the commands available")},
    {"ver", &ConsoleCommandVer, HELP("Get the version string")},
    //{"int", &ConsoleCommandParamExampleInt16, HELP("How to get a signed int16 from params list: int -321")},
    //{"u16h", &ConsoleCommandParamExampleHexUint16, HELP("How to get a hex u16 from the params list: u16h aB12")},
	{"LED=Red", &ConsoleCommandLedRed, HELP("Set LED to RED")},
	{"LED=Green", &ConsoleCommandLedGreen, HELP("Set LED to Green")},
	{"LED=Blue", &ConsoleCommandLedBlue, HELP("Set LED to Blue")},
	{"GetSetpoint", &ConsoleCommandGetSetpoint, HELP("Return current setpoint")},
	{"SetSetpoint", &ConsoleCommandSetSetpoint, HELP("Set the setpoint")},
	{"SetAngle", &ConsoleCommandSetAngle, HELP("Set the setpoint")},
	{"GetCurrentPos", &ConsoleCommandGetCurrentPos, HELP("Get current position")},
	{"SetMode", &ConsoleCommandSetMode, HELP("Set mode 0=off, 1=track position, 2=speedmode")},
	{"SetKp", &ConsoleCommandSetKp, HELP("Set Kp (*100)")},
	{"SetKi", &ConsoleCommandSetKi, HELP("Set Ki (*100)")},
	{"SetKd", &ConsoleCommandSetKd, HELP("Set Kd (*100)")},

	CONSOLE_COMMAND_TABLE_END // must be LAST
};

static eCommandResult_T ConsoleCommandComment(const char buffer[])
{
	// do nothing
	IGNORE_UNUSED_VARIABLE(buffer);
	return COMMAND_SUCCESS;
}

static eCommandResult_T ConsoleCommandHelp(const char buffer[])
{
	uint32_t i;
	uint32_t tableLength;
	eCommandResult_T result = COMMAND_SUCCESS;

    IGNORE_UNUSED_VARIABLE(buffer);

	tableLength = sizeof(mConsoleCommandTable) / sizeof(mConsoleCommandTable[0]);
	for ( i = 0u ; i < tableLength - 1u ; i++ )
	{
		ConsoleIoSendString(mConsoleCommandTable[i].name);
#if CONSOLE_COMMAND_MAX_HELP_LENGTH > 0
		ConsoleIoSendString(" : ");
		ConsoleIoSendString(mConsoleCommandTable[i].help);
#endif // CONSOLE_COMMAND_MAX_HELP_LENGTH > 0
		ConsoleIoSendString(STR_ENDLINE);
	}
	return result;
}

static eCommandResult_T ConsoleCommandParamExampleInt16(const char buffer[])
{
	int16_t parameterInt;
	eCommandResult_T result;
	result = ConsoleReceiveParamInt16(buffer, 1, &parameterInt);
	if ( COMMAND_SUCCESS == result )
	{
		ConsoleIoSendString("Parameter is ");
		ConsoleSendParamInt16(parameterInt);
		ConsoleIoSendString(" (0x");
		ConsoleSendParamHexUint16((uint16_t)parameterInt);
		ConsoleIoSendString(")");
		ConsoleIoSendString(STR_ENDLINE);
	}
	return result;
}
static eCommandResult_T ConsoleCommandParamExampleHexUint16(const char buffer[])
{
	uint16_t parameterUint16;
	eCommandResult_T result;
	result = ConsoleReceiveParamHexUint16(buffer, 1, &parameterUint16);
	if ( COMMAND_SUCCESS == result )
	{
		ConsoleIoSendString("Parameter is 0x");
		ConsoleSendParamHexUint16(parameterUint16);
		ConsoleIoSendString(STR_ENDLINE);
	}
	return result;
}

static eCommandResult_T ConsoleCommandLedRed(const char buffer[])
{
	eCommandResult_T result = COMMAND_SUCCESS;

	systemState.LEDRed = 65535;
	systemState.LEDGreen = 0;
	systemState.LEDBlue = 0;
	return result;
}

static eCommandResult_T ConsoleCommandLedGreen(const char buffer[])
{
	eCommandResult_T result = COMMAND_SUCCESS;

	systemState.LEDRed = 0;
	systemState.LEDGreen = 65535;
	systemState.LEDBlue = 0;
	return result;
}

static eCommandResult_T ConsoleCommandLedBlue(const char buffer[])
{
	eCommandResult_T result = COMMAND_SUCCESS;

	systemState.LEDRed = 0;
	systemState.LEDGreen = 0;
	systemState.LEDBlue = 65535;
	return result;
}

static eCommandResult_T ConsoleCommandGetSetpoint(const char buffer[])
{
	ConsoleIoSendString("setpoint = ");
	ConsoleSendParamInt16((uint16_t)systemState.setpoint);
	ConsoleIoSendString(STR_ENDLINE);

	eCommandResult_T result = COMMAND_SUCCESS;
	return result;
}

static eCommandResult_T ConsoleCommandSetSetpoint(const char buffer[])
{
	int16_t parameterInt;
		eCommandResult_T result;
		result = ConsoleReceiveParamInt16(buffer, 1, &parameterInt);
		if ( COMMAND_SUCCESS == result )
		{
			systemState.setpoint = (uint32_t)parameterInt;
			ConsoleIoSendString("Setpoint updated to ");
			ConsoleSendParamInt16((uint16_t)systemState.setpoint);
			ConsoleIoSendString(STR_ENDLINE);
		}

	return result;
}


static eCommandResult_T ConsoleCommandSetAngle(const char buffer[])
{
	int16_t parameterInt;
		eCommandResult_T result;
		result = ConsoleReceiveParamInt16(buffer, 1, &parameterInt);
		if ( COMMAND_SUCCESS == result )
		{
			systemState.setAngle = (float)parameterInt;
			ConsoleIoSendString("Set Angle updated to ");
			ConsoleSendParamInt16((uint16_t)systemState.setpoint);
			ConsoleIoSendString(STR_ENDLINE);
		}

	return result;
}
static eCommandResult_T ConsoleCommandGetCurrentPos(const char buffer[])
{
	ConsoleIoSendString("Current position = ");
	ConsoleSendParamInt16((uint16_t)systemState.currentPos);
	ConsoleIoSendString(STR_ENDLINE);

	eCommandResult_T result = COMMAND_SUCCESS;
	return result;
}

static eCommandResult_T ConsoleCommandSetMode(const char buffer[])
{
	int16_t parameterInt;
		eCommandResult_T result;
		result = ConsoleReceiveParamInt16(buffer, 1, &parameterInt);
		if ( COMMAND_SUCCESS == result )
		{
			systemState.mode = (uint32_t)parameterInt;
			ConsoleIoSendString("Mode updated to ");
			ConsoleSendParamInt16((uint16_t)systemState.mode);
			ConsoleIoSendString(STR_ENDLINE);
		}

	return result;
}

static eCommandResult_T ConsoleCommandSetKp(const char buffer[])
{
	int16_t parameterInt;
		eCommandResult_T result;
		result = ConsoleReceiveParamInt16(buffer, 1, &parameterInt);
		if ( COMMAND_SUCCESS == result )
		{
			PID.kp = ((uint32_t)parameterInt / 100);
			ConsoleIoSendString("Kp updated to ");
			ConsoleSendParamInt16((uint16_t)PID.kp);
			ConsoleIoSendString(STR_ENDLINE);
		}

	return result;
}

static eCommandResult_T ConsoleCommandSetKi(const char buffer[])
{
	int16_t parameterInt;
		eCommandResult_T result;
		result = ConsoleReceiveParamInt16(buffer, 1, &parameterInt);
		if ( COMMAND_SUCCESS == result )
		{
			PID.ki = ((uint32_t)parameterInt / 100);
			ConsoleIoSendString("Ki updated to ");
			ConsoleSendParamInt16((uint16_t)PID.ki);
			ConsoleIoSendString(STR_ENDLINE);
		}

	return result;
}

static eCommandResult_T ConsoleCommandSetKd(const char buffer[])
{
	int16_t parameterInt;
		eCommandResult_T result;
		result = ConsoleReceiveParamInt16(buffer, 1, &parameterInt);
		if ( COMMAND_SUCCESS == result )
		{
			PID.kd = ((uint32_t)parameterInt/100);
			ConsoleIoSendString("Kd updated to ");
			ConsoleSendParamInt16((uint16_t)PID.kd);
			ConsoleIoSendString(STR_ENDLINE);
		}

	return result;
}


static eCommandResult_T ConsoleCommandVer(const char buffer[])
{
	eCommandResult_T result = COMMAND_SUCCESS;

    IGNORE_UNUSED_VARIABLE(buffer);

	ConsoleIoSendString(VERSION_STRING);
	ConsoleIoSendString(STR_ENDLINE);
	return result;
}


const sConsoleCommandTable_T* ConsoleCommandsGetTable(void)
{
	return (mConsoleCommandTable);
}


