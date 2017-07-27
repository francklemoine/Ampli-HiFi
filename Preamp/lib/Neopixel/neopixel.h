#ifndef PREAMP_NEOP_H
#define PREAMP_NEOP_H

#include "Arduino.h"
#include "../../src/preamp.h"

void neopInit();
void neopSetBrightness();
void neopSetColor(color_t color);
void neopOff();
void neopBlink();
long neopGetLongColor(byte red, byte green, byte blue);
long increaseColor(long color);
long increaseColor(long color, basecolor_t rgb);
byte increaseBaseColor(byte color);
long decreaseColor(long color);
long decreaseColor(long color, basecolor_t rgb);
byte decreaseBaseColor(byte color);
void setColorDefault();

#endif
