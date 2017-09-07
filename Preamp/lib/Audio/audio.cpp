#include "audio.h"
#include "../../src/preamp.h"
#include "lcd.h"
#include <Wire.h>

extern audioDatas_t audioDatas;

byte incVolume() {
	byte vol = audioDatas.volume;

	if (audioDatas.volume < VOL_MAX) {
		audioDatas.volume++;
	} else {
		audioDatas.volume = VOL_MAX;
	}

	if (vol != audioDatas.volume) {
		setVolume();
		displayVolume(LARGE);
	}

	return audioDatas.volume;
}

byte decVolume() {
	byte vol = audioDatas.volume;

	if (audioDatas.volume > 0) {
		audioDatas.volume--;
	} else {
		audioDatas.volume = 0;
	}

	if (vol != audioDatas.volume) {
		setVolume();
		displayVolume(LARGE);
	}

	return audioDatas.volume;
}

void setVolume() {
	byte db = getDecibelsFromVolume(audioDatas.volume);

	byte tapPositionLoud = VOL_TAP_POS - audioDatas.volume; // permet de ne pas avoir une correction de la balance
	                                                        // provoquant un volume supérieur au volume souhaité
	byte tapPositionLow  = VOL_TAP_POS - getNearestVolumeFromDecibels(db + getBalanceDeviation());

	//DEBUG_PRINT(db);
	//DEBUG_PRINT(getBalanceDeviation());
	//DEBUG_PRINT(tapPositionLoud);
	//DEBUG_PRINT(tapPositionLow);

#ifdef USE_POTENTIOMETER_CE_PIN
	digitalWrite(POT_CE_PIN, HIGH);
#endif

	Wire.beginTransmission(VOL_ADDR_W); // transmit to device

	switch(getBalanceSlider()) {
		case MIDDLE:
			Wire.write(tapPositionLow);              // sends instruction byte (right)
			Wire.write(bitSet(tapPositionLow, 6));   // sends instruction byte (left)
			break;
		case LEFT:
			Wire.write(tapPositionLow);              // sends instruction byte (right)
			Wire.write(bitSet(tapPositionLoud, 6));  // sends instruction byte (left)
			break;
		case RIGHT:
			Wire.write(tapPositionLoud);             // sends instruction byte (right)
			Wire.write(bitSet(tapPositionLow, 6));   // sends instruction byte (left)
			break;
	}

	Wire.endTransmission(); // stop transmitting

#ifdef USE_POTENTIOMETER_CE_PIN
	digitalWrite(POT_CE_PIN, LOW);
#endif
}

#ifdef DS1882
void initConfRegister() {
#ifdef USE_POTENTIOMETER_CE_PIN
	digitalWrite(POT_CE_PIN, HIGH);
#endif
	Wire.beginTransmission(VOL_ADDR_W); // transmit to device

	Wire.write(CONF_REGISTER); // Initialize configuration register

	Wire.endTransmission(); // stop transmitting
#ifdef USE_POTENTIOMETER_CE_PIN
	digitalWrite(POT_CE_PIN, LOW);
#endif
}
#endif

// retrieve current mute state
state_t getMute() {
	if (audioDatas.volume == 0 && audioDatas.mute > 0) {
		return ON;
	} else {
		return OFF;
	}
}

void setMuteOn() {
	if (audioDatas.volume > 0) {
		audioDatas.mute = audioDatas.volume;
		audioDatas.volume = 0;
		setVolume();
		displayVolume(LARGE);
	}
}

void setMuteOff() {
	if (audioDatas.mute > 0) {
		audioDatas.volume = audioDatas.mute;
		audioDatas.mute = 0;
		setVolume();
		displayVolume(LARGE);
	}
}

/*
* -----------------------------------------------------------------
* Retourne l'écart de la balance (quelque soit le sens)
* Ex:  0 = milieu, 1,2,3...
* -----------------------------------------------------------------
*/
byte getBalanceDeviation() {
	byte deviation;

	if (audioDatas.balance >= BAL_TAP_MIDDLE) {
		deviation = audioDatas.balance - BAL_TAP_MIDDLE;
	} else {
		deviation = BAL_TAP_MIDDLE - audioDatas.balance;
	}

	return(deviation);
}

byte getDecibelsFromVolume(byte volume) {
	if (volume >= VOL_MAX) {
		return(0);
	} else if (volume == 0) {
		return(VOL_ATTN_MAX_DB);
	} else {
#ifdef DS1882
		return(VOL_TAP_POS - volume);
#else // DS1808
		return(vol2db[volume]);
#endif
	}
}

byte getNearestVolumeFromDecibels(byte decibels) {
	if (decibels >= VOL_ATTN_MAX_DB) return(0);
#ifdef DS1882
	return(VOL_TAP_POS - decibels);
#else // DS1808
	int idx = 0; // by default near first element
	int distance = abs(vol2db[idx] - decibels);
	for (byte i = 1; i < sizeof(vol2db); i++) {
		int d = abs(vol2db[i] - decibels);
		if (d < distance) {
			idx = i;
			distance = d;
		}
	}

	return idx;
#endif
}

byte turnBalanceRight() {
	byte bal = audioDatas.balance;

	if (audioDatas.balance < BAL_TAP_MAX) {
		audioDatas.balance++;
	}

	if (bal != audioDatas.balance) {
		setVolume();
		displayBalance(LARGE);
	}

	return audioDatas.balance;
}

byte turnBalanceLeft() {
	byte bal = audioDatas.balance;

	if (audioDatas.balance > 0) {
		audioDatas.balance--;
	}

	if (bal != audioDatas.balance) {
		setVolume();
		displayBalance(LARGE);
	}

	return audioDatas.balance;
}

byte getVolumeMaxStart() {
	return audioDatas.volumeMaxStart;
}

byte setVolumeMaxStart(byte newvalue) {
	if (newvalue <= VOL_MAX) audioDatas.volumeMaxStart = newvalue;
	return audioDatas.volumeMaxStart;
}
