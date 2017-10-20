#ifndef PREAMP_LCD_H
#define PREAMP_LCD_H

#include "Arduino.h"
#include "../../src/preamp.h"
#include <MenuSystem.h>

void lcdInit();
void lcdOn();
void lcdOff();
void lcdLightOff();
void lcdLightOn();
void lcdLightOff();
byte getLcdLightStatus();
void displayBasicInfos();
void displayVolume(dspfmt_t format);
void displayBalance(dspfmt_t format);
byte getBalanceVisual();
slider_t getBalanceSlider();
void displaySource();
void displayLarge(byte number);
dspfmt_t getLcdState();
void setLcdState(dspfmt_t format);
void menuInit();
void menuHandler(cmd_t command);
void menuLedStatusDependingOnContext(MenuSystem ms);
void displayMenu();
void displayMenuData(char* myData);
void onMenuSetVolMaxStart(MenuItem* pMenuItem);
void onMenuSetLedBrightness(MenuItem* pMenuItem);
void onMenuSetLedColorOn(MenuItem* pMenuItem);
void onMenuSetLedColorOff(MenuItem* pMenuItem);
void onMenuSetLedColorBlink(MenuItem* pMenuItem);
void onMenuSetLedColorDefault(MenuItem* pMenuItem);
void onMenuSetLcdBackLightSaver(MenuItem* pMenuItem);
void printBigNum(int number, int startCol, int startRow);

#endif
