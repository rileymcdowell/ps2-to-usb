
## PS2 To USB via teensy microcontroller. ##
---

The purpose of this library is to create a hardware layer which
translates a keyboard layout from querty to colemak. However that's not
really what's interesting about this project (and it's just a translation table
after all), so that's all I'll say about the "why".

Most of the code in this library was taken from the teensyduino library. You can
find updated versions of that source code at https://github.com/PaulStoffregen/cores.

The following files were created to enable this project:

1. main.cpp
2. keymap.cpp/h
3. keyState.cpp/h
4. translator.cpp/h
5. usbSender.cpp/h
6. ps2host.cpp/h
7. flags.cpp/h
8. constants.h
9. scancodes.h
10. ps2\_codes.ods


### How does this work? - Short Version ###

1. Bitbanging the PS2 protocol
2. Converting keystrokes from PS2 scancodes to an internal "keycode" representation.
3. Translating the "keycodes" to USB Usage Codes in the colemak keyboard layout.
4. Keeping track of depressed keys 
5. Sending the depressed keys while simulating a USB HID device.

### How does this work? - Long Version ###

Main.cpp is the entrypoint, which consists of an infinite loop. It starts by scheduling an 
interrupt to happen when a PS2 format scancode is detected. It waits for an interrupt to do
something. When an interrupt is detected, the ps2host.cpp file is used to read one PS2 scancode. 
If it determines that the scancode the last scancode of a complete keypress, it sets the 
ScancodeReady flag. Otherwise it just waits for the next scancode.

If a scancode is the last scancode in a key press/release, the ScancodeReady flag is set and control 
flow returns to main.cpp, which executes the keymap.cpp TranslateScancodeToKeycode function. 
This converts the PS2 scancode to an internal "keycode" representation using a series of translation tables
defined in scancodes.h. It also sets the KeycodeReady flag. 

The KeycodeReady flag signals the ProcessKey function in keyState.cpp to execute. Unlike the 
ps2 format (which sends key-up and key-down signals), the USB protocol sends the current 
state of up to 6 depressed keys, as well as information about activated modifiers. The ProcessKey function 
keeps track of the "keycodes" currently depressed using a simple binary search algorithm 
implemented on an array of "keycodes".

The TranslateToUsb() function converts the "keycodes" to USB Usage Codes in the colemak keyboard layout,
essentially changing the keyboard layout as far as the USB HID consuming device is concerned.
The "keycode" representation is simply the index into the array of USB Usage Codes in translator.cpp.
 
Last, the SendUSB function from usbSender.cpp writes the current translated USB Usage Codes to the
teensy USB port, and flickers the teensy LED during the write operation to USB.
