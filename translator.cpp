#include "translator.h"
#include "flags.h"
#include "constants.h"
#include "string.h"
#include "WProgram.h"

uint8_t usbCodes[] = {
	0, // Intentionally Not Used
	0x3A, // F1
	0x3B, // F2
	0x3C, // F3
	0x3D, // F4
	0x3E, // F5
	0x3F, // F6
	0x40, // F7
	0x41, // F8
	0x42, // F9
	
	0x43, // F10
	0x44, // F11
	0x45, // F12
	0x35, // ~ and `
	0x1E, // 1
	0x1F, // 2
	0x20, // 3
	0x21, // 4
	0x22, // 5
	0x23, // 6
	
	0x24, // 7
	0x25, // 8
	0x26, // 9
	0x27, // 0 Number
	0x2D, // - minus
	0x2E, // = and +
	0x2A, // Backspace
	0x49, // Insert
	0x4A, // Home
	0x4B, // Page Up
	
	0x2B, // Tab
	0x14, // Q
	0x1A, // W
	0x09, // E (colemak F)
	0x13, // R (colemak P)
	0x0A, // T (colemak G)
	0x0D, // Y (colemak J)
	0x0F, // U (colemak L)
	0x18, // I (colemak U)
	0x1C, // O (colemak Y)
	
	0x33, // P (colemak ;)
	0x2F, // LBrace
	0x30, // RBrace
	0x31, // Backslash
	0x4C, // Delete
	0x4D, // End
	0x4E, // Page Down
	0x2A, // CapsLock (colemak backspace again)
	0x04, // A
	0x15, // S (colemak R)
	
	0x16, // D (colemak S)
	0x17, // F (colemak T)
	0x07, // G (colemak D)
	0x0B, // H
	0x11, // J (colemak N)
	0x08, // K (colemak E)
	0x0C, // L (colemak I)
	0x12, // Semi-colon (colemak O)
	0x34, // Single and Double Quote
	0x28, // Enter
	
	0xE1, // Left Shift
	0x1D, // Z
	0x1B, // X
	0x06, // C
	0x19, // V
	0x05, // B
	0x0E, // N (colemak K)
	0x10, // M
	0x36, // , - Comma
	0x37, // . - Period
	
	0x38, // Forward Slash
	0xE5, // Right Shift
	0xE0, // Left Ctrl
	0xE3, // Left OS Key
	0xE2, // Left Alt
	0x2C, // Space
	0xE6, // Right Alt
	0xE7, // Right OS Key
	0xE4, // Right Ctrl
	0x52, // Up arrow
	
	0x50, // Left arrow
	0x51, // Down arrow
	0x4F, // Right arrow
	0x63, // Num Delete
	0x54, // Num Divide
	0x55, // Num Multiply
	0x56, // Num Subtract
	0x5F, // Num 7
	0x60, // Num 8
	0x61, // Num 9
	
	0x5C, // Num 4
	0x5D, // Num 5
	0x5E, // Num 6
	0x59, // Num 1
	0x5A, // Num 2
	0x5B, // Num 3
	0x62, // Num 0
	0x63, // Num Decimal
	0x58, // Num Enter
	0x57, // Num Add
	
	0x53, // Num Lock
	0x46, // PrintScreen
	0x47, // Scroll Lock
	0x48, // Pause/Break
	0x29, // Escape
	0x65  // Apps (Context Menu) 
};

void TranslateToUsb()
{
    // Reset the codes.
	memset(&UsbKeycodes, 0, sizeof(uint8_t) * USB_PKT_LEN);
	for (int index = 0; index < USB_PKT_LEN; index++)
	{
		uint8_t keycode = KeysToTranslate[index];
		if (keycode != 0)
		{
			// The 'keycode' is the index into the usb code lookup table.
			UsbKeycodes[index] = usbCodes[keycode];
		}
	}
	// Ready to pick-up the codes.
	UsbKeysReady = true;
}
