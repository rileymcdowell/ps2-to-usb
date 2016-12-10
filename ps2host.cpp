#include "ps2host.h"
#include "flags.h"
#include "constants.h"
#include "WProgram.h"
#include "keyState.h"
#include "keymap.h"

void ps2ClockChangedHandler()
{
	if (digitalReadFast(CLOCK) == LOW)
    {
    	ReadPS2BitNow = true;
    }
}

#define UNKNOWN -1
#define MAX_BITS 11

uint8_t bits[MAX_BITS];
uint8_t AllBytes[MAX_SCANCODE_LEN];
uint8_t bitCount = 0;

int numBytesTotal = UNKNOWN;
int byteCount = 0;

void readPs2Byte();

void readPs2Bit()
{
    bits[bitCount] = digitalReadFast(DATA);
    bitCount++;
    if (bitCount == MAX_BITS)
    {
        readPs2Byte();
        bitCount = 0;
    }
}

void readFirstByte(uint8_t code);
void readSecondByte(uint8_t code);
void readThirdByte(uint8_t code);

void readPs2Byte()
{
    uint8_t code = 0;
    
	// Read 8 bits with the
	// least significant bit first.
	for (int index = 8; index >= 1; index--)
	{
		code = code << 1;
		code += bits[index] ? 1 : 0;
	}

	AllBytes[byteCount] = code;
	byteCount++;
	
	
	if (numBytesTotal == UNKNOWN)
	{
		if (byteCount == 1)
		{
			readFirstByte(code);
		}
		else if (byteCount == 2)
		{
			readSecondByte(code);
		}
		else if (byteCount == 3)
		{
			readThirdByte(code);
		}
	}
	if (byteCount < numBytesTotal)
	{
		return; // We know how many bytes we need, so just keep reading more.
	}
	
	if (byteCount == numBytesTotal) // Scancode complete.
	{
	    memcpy(&LastScancode, &AllBytes, sizeof(uint8_t) * MAX_SCANCODE_LEN);
		ScancodeReady = true;

		// Reset.
		numBytesTotal = UNKNOWN;
	    byteCount = 0;
	}
}

void readFirstByte(uint8_t code)
{
	switch (code)
	{
		case 0xF0: // F0 start is always a 2 byte break code.
			numBytesTotal = 2; 
			break;
		case 0xE0: // Pass to 2nd byte.
			numBytesTotal = UNKNOWN;
			break;
		case 0xE1:
			numBytesTotal = 8; // Must be pause key.
			break;
		default: // Anything else must be 1 byte makecode.
			numBytesTotal = 1;
			break;
	}
}

void readSecondByte(uint8_t code)
{
	switch (code)
	{
		case 0xF0:
			numBytesTotal = UNKNOWN; // Pass to 3rd byte.
			break;
		default: // Anything else must be 2 bytes total.
			numBytesTotal = 2;
			break;
	}
}

void readThirdByte(uint8_t code)
{
	switch (code)
	{
		default: // Must be a 3 byte code.
			numBytesTotal = 3;
			break;
	}
}
