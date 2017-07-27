#include "audio.h"
#include "../../src/preamp.h"
#include "lcd.h"
#include <Wire.h>

extern audioDatas_t audioDatas;

byte incVolume() {
	byte vol = audioDatas.volume;

	/*if (audioDatas.volume < VOL_TAP_MAX - audioDatas.volumeStepDefault) {
		audioDatas.volume += audioDatas.volumeStepDefault;
	} else {
		audioDatas.volume = VOL_TAP_MAX;
	}*/
	if (audioDatas.volume < VOL_TAP_MAX) {
		audioDatas.volume++;
	} else {
		audioDatas.volume = VOL_TAP_MAX;
	}

	if (vol != audioDatas.volume) {
		setVolume();
		displayVolume(LARGE);
	}

	return audioDatas.volume;
}

byte decVolume() {
	byte vol = audioDatas.volume;
	/*if (audioDatas.volume > audioDatas.volumeStepDefault) {
		audioDatas.volume -= audioDatas.volumeStepDefault;
	} else {
		audioDatas.volume = 0;
	}*/
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

	byte tapPositionLoud = VOL_TAP_MAX - audioDatas.volume; // permet de ne pas avoir une correction de la balance
	                                                       // provoquant un volume supérieur au volume souhaité
	byte tapPositionLow = VOL_TAP_MAX - getNearestVolumeFromDecibels(db + getBalanceDeviation());

	//DEBUG_PRINT(db);
	//DEBUG_PRINT(getBalanceDeviation());
	//DEBUG_PRINT(tapPositionLoud);
	//DEBUG_PRINT(tapPositionLow);

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
}

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
	if (volume >= sizeof(vol2db)) return(vol2db[sizeof(vol2db) - 1]);

	return(vol2db[volume]);
}

byte getNearestVolumeFromDecibels(byte decibels) {
	if (decibels > vol2db[0]) return(0);

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
}

// 0 <=  audioDatas.balance  <= 126 - default=63 (middle)
byte turnBalanceRight() {
	byte bal = audioDatas.balance;

	/*if (audioDatas.balance < BAL_TAP_MIDDLE && audioDatas.balance + audioDatas.balanceStepDefault > BAL_TAP_MIDDLE) {
		audioDatas.balance = BAL_TAP_MIDDLE;
	} else if (audioDatas.balance <= BAL_TAP_MAX - audioDatas.balanceStepDefault) {
		audioDatas.balance += audioDatas.balanceStepDefault;
	}*/
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

	/*if (audioDatas.balance > BAL_TAP_MIDDLE && audioDatas.balance - audioDatas.balanceStepDefault < BAL_TAP_MIDDLE) {
		audioDatas.balance = BAL_TAP_MIDDLE;
	} else if (audioDatas.balance >= audioDatas.balanceStepDefault) {
		audioDatas.balance -= audioDatas.balanceStepDefault;
	}*/
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
	if (newvalue <= VOL_TAP_MAX) audioDatas.volumeMaxStart = newvalue;
	return audioDatas.volumeMaxStart;
}

/*byte getVolumeStepDefault() {
	return audioDatas.volumeStepDefault;
}*/

/*byte setVolumeStepDefault(byte newvalue) {
	if (newvalue >= VOL_STEP_DEFAULT_MIN && newvalue <= VOL_STEP_DEFAULT_MAX) {
		audioDatas.volumeStepDefault = newvalue;
	} else {
		audioDatas.volumeStepDefault = VOL_STEP_DEFAULT;
	}
	return audioDatas.volumeStepDefault;
}*/

/*byte getBalanceStepDefault() {
	return audioDatas.balanceStepDefault;
}*/

/*byte setBalanceStepDefault(byte newvalue) {
	if (newvalue >= BAL_STEP_DEFAULT_MIN && newvalue <= BAL_STEP_DEFAULT_MAX) {
		audioDatas.volumeStepDefault = newvalue;
	} else {
		audioDatas.volumeStepDefault = BAL_STEP_DEFAULT;
	}
	return audioDatas.balanceStepDefault;
}*/
