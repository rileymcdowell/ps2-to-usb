#include "keymap.h"
#include "scancodes.h"
#include "stdint.h"
#include "flags.h"

uint8_t keycodes[] = { 
	0, // Intentionally Not Used
	1, // F1
	2, // F2
	3, // F3
	4, // F4
	5, // F5
	6, // F6
	7, // F7
	8, // F8
	9, // F9
	
	10, // F10
	11, // F11
	12, // F12
	13, // ~ and `
	14, // 1
	15, // 2
	16, // 3
	17, // 4
	18, // 5
	19, // 6
	
	20, // 7
	21, // 8
	22, // 9
	23, // 0 Number
	24, // - minus
	25, // = and +
	26, // Backspace
	27, // Insert
	28, // Home
	29, // Page Up
	
	30, // Tab
	31, // Q
	32, // W
	33, // E
	34, // R
	35, // T
	36, // Y
	37, // U
	38, // I
	39, // O Letter
	
	40, // P
	41, // LBrace
	42, // RBrace
	43, // Backslash
	44, // Delete
	45, // End
	46, // Page Down
	47, // CapsLock
	48, // A
	49, // S
	
	50, // D
	51, // F
	52, // G
	53, // H
	54, // J
	55, // K
	56, // L
	57, // Semi-colon
	58, // Single and Double Quote
	59, // Enter
	
	60, // Left Shift
	61, // Z
	62, // X
	63, // C
	64, // V
	65, // B
	66, // N
	67, // M
	68, // , - Comma
	69, // . - Period
	
	70, // Forward Slash
	71, // Right Shift
	72, // Left Ctrl
	73, // Left OS Key
	74, // Left Alt
	75, // Space
	76, // Right Alt
	77, // Right OS Key
	78, // Right Ctrl
	79, // Up arrow
	
	80, // Left arrow
	81, // Down arrow
	82, // Right arrow
	83, // Num Delete
	84, // Num Divide
	85, // Num Multiply
	86, // Num Subtract
	87, // Num 7
	88, // Num 8
	89, // Num 9
	
	90, // Num 4
	91, // Num 5
	92, // Num 6
	93, // Num 1
	94, // Num 2
	95, // Num 3
	96, // Num 0
	97, // Num Decimal
	98, // Num Enter
	99, // Num Add
	
	100, // Num Lock
	101, // PrintScreen
	102, // Scroll Lock
	103, // Pause/Break
	104, // Escape
	105  // Apps (Context Menu) 
};

int processScancode(uint8_t* scancode);
int firstByte(uint8_t* scancode);
int secondByte(uint8_t* scancode);
int thirdByte(uint8_t* scancode);

void TranslateScancodeToKeycode()
{
	// If Make Code, assign positive.
	// If Break Code, assign negative.
	// Start by reading the first byte.
	LastKeycode = processScancode(LastScancode);
	KeycodeReady = true;
}

int processScancode(uint8_t* scancode)
{
	return firstByte(scancode);
}

int firstByte(uint8_t* scancode)
{
	uint8_t byteOne = scancode[0];
	switch (byteOne)
	{
		case 0xF0: // Two byte breakcode
			return -scancodes1[scancode[1]];
		case 0xE0: // Must process more bytes to determine.
			return secondByte(scancode);
		case 0xE1: // This must be the Pause key.
			return 103;
		case 0x00: // Keyboard error.
		case 0xAA: // Basic Assurance Test (Keyboard working).
		case 0xEE: // Echo back from keyboard.
		case 0xF1: // Reply to passwored not installed ???
		case 0xFA: // Keyboard acknowledge.
		case 0xFC: // Transmit error.
		case 0xFD: // Internal failure.
		case 0xFE: // Please resend.
		case 0xFF: // Keyboard error.
			return 0;
		default: // A one byte makecode.
			return scancodes1[byteOne];
	}
}

int secondByte(uint8_t* scancode)
{
	uint8_t byteTwo = scancode[1];
	switch (byteTwo)
	{
		case 0x12: 
			if (scancode[0] == 0xE0) // 0xE012 always ignored.
			{
				return 0;
			}
			else
			{
				return scancode[byteTwo];
			}
		case 0xF0: // Must process more bytes to determine.
			return thirdByte(scancode);
		default: // This is a two byte makecode.
			return scancodes2[byteTwo];
	}
}

int thirdByte(uint8_t* scancode)
{
	uint8_t byteThree = scancode[2];
	switch (byteThree)
	{
		case 0x12: // 0xE0F012 always ignored.
			return 0; 
		default: // Three byte breakcode.
			return -scancodes2[byteThree];
	}
}
