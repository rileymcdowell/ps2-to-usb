#include "keyState.h"
#include "WProgram.h"
#include "flags.h"
#include "constants.h"

#define KEY_ARR_SIZE 128

uint8_t keysDown[KEY_ARR_SIZE];
uint8_t usbSubset[USB_PKT_LEN];
int totalKeys = 0;
// Temp array for use when array copying.
uint8_t temp[KEY_ARR_SIZE];

// Declare internal functions.
void stdInsert(uint8_t keycode);
void stdRemove(uint8_t keycode);
void modInsert(uint8_t keycode);
void modRemove(uint8_t keycode);
int binarySearch(uint8_t keycode, int minSearchIdx, int maxSearchIdx);
void removeAt(int index);
void insertAt(int index, uint8_t keycode);
void setCurrentKeys(void);

void ProcessKey()
{
	// Obtain keycode for processing.
	int keycode = LastKeycode;
	
	void (*processFunPtr)(uint8_t);
	switch (abs(keycode))
	{
	    case 72: // Left Ctrl
		case 60: // Left Shift
		case 74: // Left Alt
		case 73: // Left OS
		case 78: // Right Ctrl
		case 71: // Right Shift
		case 76: // Right Alt
		case 77: // Right OS
			processFunPtr = (keycode > 0) ? &modInsert : &modRemove;
			break;
		default:
			processFunPtr = (keycode > 0) ? &stdInsert : &stdRemove;
			break;
	}
	
	(*processFunPtr)(keycode > 0 ? keycode : -keycode);
	
	setCurrentKeys();
	TranslateReady = true;
}

// Set up to 6 keys as 'down' right now.
void setCurrentKeys()
{
	// Clear the array.
	memset(&KeysToTranslate, 0, sizeof(uint8_t) * USB_PKT_LEN);
	uint8_t numToTranslate = totalKeys >= USB_PKT_LEN ? USB_PKT_LEN : totalKeys;
	// Copy the depressed keys into it.
	memcpy(&KeysToTranslate, &keysDown, sizeof(uint8_t) * numToTranslate);
}

void stdInsert(uint8_t keycode)
{
	int index = binarySearch(keycode, 0, totalKeys);
	if (index >= 0)
	{
		return; // Alreay inserted.
	}
	else 
	{
		// Value plus one is negative of new position.
		insertAt(-1 * (index + 1), keycode);
		totalKeys++;
	}
}

void stdRemove(uint8_t keycode)
{
	int index = binarySearch(keycode, 0, totalKeys);
	if (index >= 0)
	{
		removeAt(index);
		totalKeys--;
	}
	else
	{
		return; // Nothing to remove.
	}
}

// Stolen from java.
int binarySearch(uint8_t keycode, int minSearchIdx, int maxSearchIdx)
{
	int low = minSearchIdx;
	int high = maxSearchIdx - 1;
	
	while (low <= high)
	{
		int mid = (low + high) / 2;
		uint8_t midVal = keysDown[mid];
		
		if (midVal < keycode)
		{
			low = mid + 1;
		}
		else if (midVal > keycode)
		{
			high = mid - 1;
		}
		else
		{
			return mid; // Exact match.
		}
	}
	return -1 * (low + 1); // Would be insertion point.
}

void modInsert(uint8_t keycode)
{
	switch (keycode)
	{
		case 72: // Left Ctrl
			ModifierMask |= 1;
			break;
		case 60: // Left Shift
			ModifierMask |= 2;
			break;
		case 74: // Left Alt
			ModifierMask |= 4;
			break;
		case 73: // Left OS
			ModifierMask |= 8;
			break;
		case 78: // Right Ctrl
			ModifierMask |= 16;
			break;
		case 71: // Right Shift
			ModifierMask |= 32;
			break;
		case 76: // Right Alt
			ModifierMask |= 64;
			break;
		case 77: // Right OS
			ModifierMask |= 128;
			break;
	}
}

void modRemove(uint8_t keycode)
{
	switch (keycode)
	{
		case 72: // Left Ctrl
			ModifierMask ^= 1;
			break;
		case 60: // Left Shift
			ModifierMask ^= 2;
			break;
		case 74: // Left Alt
			ModifierMask ^= 4;
			break;
		case 73: // Left OS
			ModifierMask ^= 8;
			break;
		case 78: // Right Ctrl
			ModifierMask ^= 16;
			break;
		case 71: // Right Shift
			ModifierMask ^= 32;
			break;
		case 76: // Right Alt
			ModifierMask ^= 64;
			break;
		case 77: // Right OS
			ModifierMask ^= 128;
			break;
	}
}

void insertAt(int index, uint8_t keycode)
{
	// Before index
	memcpy(&temp, &keysDown, index * sizeof(uint8_t) - 1);
	// At index
	temp[index] = keycode; // Insert the value.
	// After index
	memcpy(&temp[index + 1], &keysDown[index], (KEY_ARR_SIZE - index - 1) * sizeof(uint8_t));
	memcpy(&keysDown, &temp, KEY_ARR_SIZE);
}

void removeAt(int index)
{
	memcpy(&temp, &keysDown, index * sizeof(uint8_t) - 1);
	memcpy(&temp[index], &keysDown[index + 1], (KEY_ARR_SIZE - index - 1) * sizeof(uint8_t));
	memcpy(&keysDown, &temp, KEY_ARR_SIZE);
}
