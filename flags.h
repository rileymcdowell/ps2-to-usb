#pragma once

#include "stdint.h"

// Volatile because tied to interrupts.
extern volatile bool ReadPS2BitNow;

extern bool ScancodeReady;
extern bool KeycodeReady;
extern bool TranslateReady;
extern bool UsbKeysReady;

extern uint8_t LastScancode[];
extern int LastKeycode;
extern uint8_t KeysToTranslate[];
extern uint8_t ModifierMask;
extern uint8_t UsbKeycodes[];
