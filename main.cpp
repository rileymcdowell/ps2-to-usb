#include "mk20dx128.h"
#include "WProgram.h"
#include "ps2host.h"
#include "flags.h"
#include "constants.h"
#include "keymap.h"
#include "keyState.h"
#include "translator.h"
#include "usbSender.h"

void setup()
{
	pinMode(CLOCK, INPUT);
	pinMode(DATA, INPUT);
	pinMode(LED, OUTPUT);

	digitalWriteFast(LED, LOW);
	
	delay(3000);
	
    // Trigger when signal changes level.
    attachInterrupt(CLOCK, ps2ClockChangedHandler, CHANGE);
}

void loop()
{
	if (ReadPS2BitNow) // Priority 1
	{
		readPs2Bit();
		ReadPS2BitNow = false;
		return;
	}
	if (ScancodeReady) // Priority 2
	{
		TranslateScancodeToKeycode();
		ScancodeReady = false;
		return;
	}
	if (KeycodeReady) // Priority 3
	{
		ProcessKey();
		KeycodeReady = false;
		return;
	}
	if (TranslateReady) // Priority 4
	{
		TranslateToUsb();
		TranslateReady = false;
		return;
	}
	if (UsbKeysReady) // Priority 5
	{
		SendUSB();
		UsbKeysReady = false;
		return;
	}
}

int main(void)
{
	setup();
    
    while (true)
    {
    	loop();
    }
}
