#ifndef PREAMP_SOURCES_H
#define PREAMP_SOURCES_H

#include "Arduino.h"
#include "../../src/preamp.h"
#include "lcd.h"

void clearSource();
void setSource(sources_t sx);
void setSource(sources_t sx, boolean force);
sources_t getSource();

#endif
