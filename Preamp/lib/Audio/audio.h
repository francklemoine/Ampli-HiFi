#ifndef PREAMP_AUDIO_H
#define PREAMP_AUDIO_H

#include "Arduino.h"
#include "../../src/preamp.h"

void audioInit();
byte incVolume();
byte decVolume();
void setVolume();
void initConfRegister();
state_t getMute();
void setMuteOn();
void setMuteOff();
byte getBalanceDeviation();
byte getDecibelsFromVolume(byte volume);
byte getNearestVolumeFromDecibels(byte decibels);
byte turnBalanceRight();
byte turnBalanceLeft();
byte getVolumeMaxStart();
byte setVolumeMaxStart(byte newvalue);

#endif
