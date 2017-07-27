#include "sources.h"
#include "../../src/preamp.h"
#include "lcd.h"

extern audioDatas_t audioDatas;

void clearSource() {
	byte source = audioDatas.source & B00000001;
	digitalWrite(LATCH_595_PIN, LOW);
	shiftOut(DATAS_595_PIN, CLOCK_595_PIN, LSBFIRST, source);
	digitalWrite(LATCH_595_PIN, HIGH);
	audioDatas.source = source;
}

void setSource(sources_t sx) {
	setSource(sx, false);
}

void setSource(sources_t sx, boolean force) {
	if (sx != getSource() || force) {
		byte source;
		clearSource();
		delay(10);
		if (sx == S1) {
			source = audioDatas.source | B10000000;
		} else if (sx == S2) {
			source = audioDatas.source | B01000000;
		} else if (sx == S3) {
			source = audioDatas.source | B00100000;
		} else if (sx == S4) {
			source = audioDatas.source | B00010000;
		} else if (sx == S5) {
			source = audioDatas.source | B00001000;
		} else if (sx == S6) {
			source = audioDatas.source | B00000100;
		} else if (sx == S7) {
			source = audioDatas.source | B00000010;
		}
		digitalWrite(LATCH_595_PIN, LOW);
		shiftOut(DATAS_595_PIN, CLOCK_595_PIN, LSBFIRST, source);
		digitalWrite(LATCH_595_PIN, HIGH);
		audioDatas.source = source;
		displaySource();
	}
}

sources_t getSource() {
	if (bitRead(audioDatas.source, 7)) return S1;
	if (bitRead(audioDatas.source, 6)) return S2;
	if (bitRead(audioDatas.source, 5)) return S3;
	if (bitRead(audioDatas.source, 4)) return S4;
	if (bitRead(audioDatas.source, 3)) return S5;
	if (bitRead(audioDatas.source, 2)) return S6;
	if (bitRead(audioDatas.source, 1)) return S7;
	return S_UNDEF;
}
