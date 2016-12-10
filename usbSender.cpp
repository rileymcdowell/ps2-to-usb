#include "flags.h"
#include "constants.h"
#include "usb_keyboard.h"
#include "stdint.h"
#include "WProgram.h"

void SendUSB()
{
	digitalWriteFast(LED, HIGH);
#ifdef USB_HID
	memcpy(&keyboard_keys, &UsbKeycodes, sizeof(uint8_t) * USB_PKT_LEN);
	keyboard_modifier_keys = ModifierMask;
	usb_keyboard_send();
#else

	for (int index = 0; index < USB_PKT_LEN; index++)
	{
		Serial.print(UsbKeycodes[index]);
	}
	Serial.println();
#endif

	digitalWriteFast(LED, LOW);
}
