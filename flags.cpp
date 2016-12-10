#include "flags.h"
#include "constants.h"
#include "stdint.h"

volatile bool ReadPS2BitNow;

bool ScancodeReady;
bool KeycodeReady;
bool TranslateReady;
bool UsbKeysReady;

uint8_t LastScancode[MAX_SCANCODE_LEN];
int LastKeycode;
uint8_t KeysToTranslate[USB_PKT_LEN];
uint8_t ModifierMask;
uint8_t UsbKeycodes[USB_PKT_LEN];
