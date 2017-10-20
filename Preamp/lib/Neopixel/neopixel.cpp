#include "neopixel.h"
#include "../../src/preamp.h"
#include <Adafruit_NeoPixel.h>

extern audioDatas_t audioDatas;
extern Adafruit_NeoPixel neopx;

void neopInit() {
	neopx.begin();
	neopSetBrightness();
}

void neopSetBrightness() {
	neopx.setBrightness(audioDatas.neopxBrightness);
	neopx.show();
}

void neopSetColor(color_t color) {
	switch(color) {
		case COLOR_ON:
			neopx.setPixelColor(0, neopx.Color(G_COLOR(audioDatas.neopxOn), R_COLOR(audioDatas.neopxOn), B_COLOR(audioDatas.neopxOn)));
			break;
		case COLOR_OFF:
			neopx.setPixelColor(0, neopx.Color(G_COLOR(audioDatas.neopxOff), R_COLOR(audioDatas.neopxOff), B_COLOR(audioDatas.neopxOff)));
			break;
		case COLOR_BLINK:
			neopx.setPixelColor(0, neopx.Color(G_COLOR(audioDatas.neopxBlink), R_COLOR(audioDatas.neopxBlink), B_COLOR(audioDatas.neopxBlink)));
			break;
		default:
			neopx.setPixelColor(0, neopx.Color(0, 0, 0));
			break;
	}
	neopx.show();
}

void neopOff() {
	neopSetColor(COLOR_OFF_0);
	digitalWrite(NEOPIXEL_GND_PIN, LOW);    // disconnect LED K
}

long neopGetLongColor(byte red, byte green, byte blue) {
	unsigned long color = 0;
	color = ((long)red << 16) + ((long)green << 8) + (long)blue;
	return(color);
}

long increaseColor(long color) {
	const byte increment = 0xF; //0x3F;

	byte red   = R_COLOR(color);
	byte green = G_COLOR(color);
	byte blue  = B_COLOR(color);
	if (blue + increment <= 0xFF) {
		blue += increment;
	} else if (green + increment <= 0xFF) {
		blue = 0;
		green += increment;
	} else if (red + increment <= 0xFF) {
		blue = 0;
		green = 0;
		red += increment;
	} else {
		blue = increment;
		green = 0;
		red = 0;
	}
	return neopGetLongColor(red, green, blue);
}

long increaseColor(long color, basecolor_t rgb) {
	byte red   = R_COLOR(color);
	byte green = G_COLOR(color);
	byte blue  = B_COLOR(color);
	switch (rgb) {
		case RED:
			red = increaseBaseColor(red);
			break;
		case GREEN:
			green = increaseBaseColor(green);
			break;
		case BLUE:
			blue = increaseBaseColor(blue);
			break;
		default:
			break;
	}
	return neopGetLongColor(red, green, blue);
}

byte increaseBaseColor(byte color) {
	// 0x00, 0x10, ..., 0xD0, 0xE0, 0xF0, 0xFF
	byte c = color;

	if (c == 0xFF) {
		c = 0;
	} else if (c <= 0xE0) {
		c += 0x10;
	} else {
		c = 0xFF;
	}
	return c;
}

long decreaseColor(long color) {
	const byte increment = 0xF; //0x3F;

	byte red   = R_COLOR(color);
	byte green = G_COLOR(color);
	byte blue  = B_COLOR(color);
	if (blue - increment >= 0) {
		blue -= increment;
	} else if (green - increment >= 0) {
		blue = 0xFF;
		green -= increment;
	} else if (red - increment >= 0) {
		blue = 0xFF;
		green = 0xFF;
		red -= increment;
	} else {
		blue = 0xFF;
		green = 0xFF;
		red = 0xFF;
	}
	return neopGetLongColor(red, green, blue);
}

long decreaseColor(long color, basecolor_t rgb) {
	byte red   = R_COLOR(color);
	byte green = G_COLOR(color);
	byte blue  = B_COLOR(color);
	switch (rgb) {
		case RED:
			red = decreaseBaseColor(red);
			break;
		case GREEN:
			green = decreaseBaseColor(green);
			break;
		case BLUE:
			blue = decreaseBaseColor(blue);
			break;
		default:
			break;
	}
	return neopGetLongColor(red, green, blue);
}

byte decreaseBaseColor(byte color) {
	// 0x00, 0x10, ..., 0xD0, 0xE0, 0xF0, 0xFF
	byte c = color;
	if (c == 0) {
		c = 0xFF;
	} else if (c <= 0x20) {
		c -= 0x10;
	} else {
		c = 0;
	}
	return c;
}

void setColorDefault() {
	audioDatas.neopxOn    = neopGetLongColor(NEOPIXEL_ON_R, NEOPIXEL_ON_G, NEOPIXEL_ON_B);
	audioDatas.neopxOff   = neopGetLongColor(NEOPIXEL_OFF_R, NEOPIXEL_OFF_G, NEOPIXEL_OFF_B);
	audioDatas.neopxBlink = neopGetLongColor(NEOPIXEL_BLINK_R, NEOPIXEL_BLINK_G, NEOPIXEL_BLINK_B);
	audioDatas.neopxOnDefault    = 1;
	audioDatas.neopxOffDefault   = 1;
	audioDatas.neopxBlinkDefault = 1;
}
