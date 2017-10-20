#include "storage.h"
#include "../../src/preamp.h"
#include "lcd.h"
#include "neopixel.h"
#include <EEPROM.h>  // Librairie pour le stockage de données en mémoire Eeprom interne

extern audioDatas_t audioDatas;

void restoreAudioDatas() {
	EEPROM.get(ADDR_MEM_BASE, audioDatas);

	// volume (volume max au démarrage)
	if (audioDatas.volumeMaxStart > VOL_MAX) // Locations that have never been written to have the value of 255
		audioDatas.volumeMaxStart = VOL_TAP_POS - VOL_TAP_STARTUP;

	// volume
	if (audioDatas.volume > VOL_MAX) // Locations that have never been written to have the value of 255
		audioDatas.volume = 0;
	if (audioDatas.volume > audioDatas.volumeMaxStart)
		audioDatas.volume = audioDatas.volumeMaxStart;

	// balance
	if (audioDatas.balance > BAL_TAP_MAX) // Locations that have never been written to have the value of 255
		audioDatas.balance = BAL_TAP_MIDDLE;

	// source (ne pas actionner le relais HP-Phone)
	audioDatas.source &= B11111110;

	audioDatas.mute = 0;

	if (audioDatas.neopxBrightness > NEOPIXEL_BRIGHTNESS_MAX) // Locations that have never been written to have the value of 255
		audioDatas.neopxBrightness = NEOPIXEL_BRIGHTNESS;

	if (audioDatas.neopxOnDefault > 0)
		audioDatas.neopxOn = neopGetLongColor(NEOPIXEL_ON_R, NEOPIXEL_ON_G, NEOPIXEL_ON_B);
	if (audioDatas.neopxOffDefault > 0)
		audioDatas.neopxOff = neopGetLongColor(NEOPIXEL_OFF_R, NEOPIXEL_OFF_G, NEOPIXEL_OFF_B);
	if (audioDatas.neopxBlinkDefault > 0)
		audioDatas.neopxBlink = neopGetLongColor(NEOPIXEL_BLINK_R, NEOPIXEL_BLINK_G, NEOPIXEL_BLINK_B);

	audioDatas.neopxStatus = COLOR_ON;

	if (audioDatas.lcdBackLightSaver > 0) { // Locations that have never been written to have the value of 255
		audioDatas.lcdBackLightSaver = 1;
	}
}

void saveAudioDatas() {
	EEPROM.put(ADDR_MEM_BASE, audioDatas);
}
