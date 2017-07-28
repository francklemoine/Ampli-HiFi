#include "lcd.h"
#include "../../src/preamp.h"
#include "sources.h"
#include "neopixel.h"
#include <Wire.h>
#include <MenuSystem.h>
#include <LiquidCrystal_PCF8574.h>

extern audioDatas_t audioDatas;
extern unsigned long lastLcdDisplay;
extern unsigned long lastLargeLcdDisplay;

dspfmt_t lcdState = BASIC;
byte lcdLightStatus = 0;

const char* LCD_AFF_SRC[8] = {
	[S_UNDEF] = "     ",
	[S1]      = "Cd   ",
	[S2]      = "Pc   ",
	[S3]      = "Tv   ",
	[S4]      = "Dvd  ",
	[S5]      = "Tuner",
	[S6]      = "Aux1 ",
	[S7]      = "Aux2 "
};

// Define the bit patters for each of our custom chars. These
// are 5 bits wide and 8 dots deep
uint8_t custChar[8][8] = {
	{31, 31, 31, 0, 0, 0, 0, 0},      // Small top line - 0
	{0, 0, 0, 0, 0, 31, 31, 31},      // Small bottom line - 1
	{31, 0, 0, 0, 0, 0, 0, 31},       // Small lines top and bottom - 2
	{0, 0, 0, 0, 0, 0,  0, 31},       // Thin bottom line - 3
	{31, 31, 31, 31, 31, 31, 15, 7},  // Left bottom chamfer full - 4
	{28, 30, 31, 31, 31, 31, 31, 31}, // Right top chamfer full - 5
	{31, 31, 31, 31, 31, 31, 30, 28}, // Right bottom chamfer full - 6
	{7, 15, 31, 31, 31, 31, 31, 31},  // Left top chamfer full - 7
};

// Define our numbers 0 thru 9
// 254 is blank and 255 is the "Full Block"
uint8_t bigNums[10][6] = {
	{7, 0, 5, 4, 1, 6},         //0
	{0, 5, 254, 1, 255, 1},     //1
	{0, 2, 5, 7, 3, 1},         //2
	{0, 2, 5, 1, 3, 6},         //3
	{7, 3, 255, 254, 254, 255}, //4
	{7, 2, 0, 1, 3, 6},         //5
	{7, 2, 0, 4, 3, 6},         //6
	{0, 0, 5, 254, 7, 254},     //7
	{7, 2, 5, 4, 3, 6},         //8
	{7, 2, 5, 1, 3, 6},         //9
};

LiquidCrystal_PCF8574 lcd(LCD_I2C_ADDR);  // set the LCD address

const char *MU_ROOT          = "Menu";
const char *MU1_AUD          = "Audio";
const char *MU2_LED          = "Led";
const char *MU3_LCD          = "Lcd";

const char *MI_VOLSMAX       = "V.Startup";
//const char *MI_VOLSTEP       = "V.Step";
//const char *MI_BALSTEP       = "B.Step";

const char *MI_BRIGHTP       = "Brightness+";
const char *MI_BRIGHTM       = "Brightness-";

const char *MI_COLOR_ON_R    = "Col.On R";
const char *MI_COLOR_ON_G    = "Col.On G";
const char *MI_COLOR_ON_B    = "Col.On B";

const char *MI_COLOR_OFF_R    = "Col.Off R";
const char *MI_COLOR_OFF_G    = "Col.Off G";
const char *MI_COLOR_OFF_B    = "Col.Off B";

const char *MI_COLOR_BLK_R    = "Col.Blk R";
const char *MI_COLOR_BLK_G    = "Col.Blk G";
const char *MI_COLOR_BLK_B    = "Col.Blk B";

const char *MI_COLOR_RAZ     = "Col.RAZ";

const char *MI_LCD_BL_SAVER  = "BackLightSav";

MenuSystem ms;
Menu mm(MU_ROOT);

Menu mu1(MU1_AUD);
MenuItem mu1_mi1(MI_VOLSMAX);
//MenuItem mu1_mi2(MI_VOLSTEP);
//MenuItem mu1_mi3(MI_BALSTEP);

Menu mu2(MU2_LED);

MenuItem mu2_mi01(MI_BRIGHTP);
MenuItem mu2_mi02(MI_BRIGHTM);

MenuItem mu2_mi30(MI_COLOR_ON_R);
MenuItem mu2_mi31(MI_COLOR_ON_G);
MenuItem mu2_mi32(MI_COLOR_ON_B);

MenuItem mu2_mi33(MI_COLOR_OFF_R);
MenuItem mu2_mi34(MI_COLOR_OFF_G);
MenuItem mu2_mi35(MI_COLOR_OFF_B);

MenuItem mu2_mi36(MI_COLOR_BLK_R);
MenuItem mu2_mi37(MI_COLOR_BLK_G);
MenuItem mu2_mi38(MI_COLOR_BLK_B);

MenuItem mu2_mi50(MI_COLOR_RAZ);

Menu mu3(MU3_LCD);

MenuItem mu3_mi01(MI_LCD_BL_SAVER);

void lcdInit() {
	Wire.beginTransmission(LCD_I2C_ADDR);
	lcd.begin(16, 2); // initialize the lcd

	// Create custom character map (8 characters only!)
	for (byte cnt = 0; cnt < sizeof(custChar) / 8; cnt++) {
		lcd.createChar(cnt, custChar[cnt]);
	}

	lcdOff();
}

void lcdOn() {
	displayBasicInfos();
	lcdLightOn();
}

void lcdOff() {
	lcd.home();
	lcd.clear();
	lcdLightOff();
}

void lcdLightOff() {
	lcdLightStatus = 0;
	lcd.setBacklight(lcdLightStatus);
}

void lcdLightOn() {
	lcdLightStatus = 1;
	lcd.setBacklight(lcdLightStatus);
}

byte getLcdLightStatus() {
	return lcdLightStatus;
}


void displayBasicInfos() {
	setLcdState(BASIC);
	lcd.clear();
	displayVolume(BASIC);
	displayBalance(BASIC);
	displaySource();
}

void displayVolume(dspfmt_t format) {
	switch(format) {
		case BASIC:
			lcd.setCursor(0, 0);        // digit-1, line-1
			lcd.print("vol: ");
			lcd.setCursor(5, 0);        // digit-6, line-1
			lcd.print("  ");
			lcd.setCursor(5, 0);        // digit-6, line-1
			lcd.print(audioDatas.volume);
			break;
		case LARGE:
			lcd.clear();
			lcd.setCursor(1, 1);        // digit-2, line-1
			lcd.print("VOL");
			displayLarge(audioDatas.volume);
			break;
		default:
			break;
	}
}

void displayBalance(dspfmt_t format) {
	switch(format) {
		case BASIC:
			lcd.setCursor(0, 1);        // digit-1, line-2
			lcd.print("bal: ");
			lcd.setCursor(5, 1);        // digit-6, line-2
			lcd.print("      ");
			lcd.setCursor(5, 1);        // digit-6, line-2
			lcd.print(getBalanceVisual());
			if (getBalanceSlider() == LEFT)  lcd.print((char)B01111111);
			if (getBalanceSlider() == RIGHT) lcd.print((char)B01111110);
			break;
		case LARGE:
			lcd.clear();
			lcd.setCursor(1, 1);        // digit-2, line-1
			lcd.print("BAL");
			displayLarge(getBalanceVisual());
			if (getBalanceSlider() == LEFT)  lcd.print(" L");
			if (getBalanceSlider() == RIGHT) lcd.print(" R");
			break;
		default:
			break;
	}
}

byte getBalanceVisual() {
	if (audioDatas.balance < BAL_TAP_MIDDLE) {
		return(BAL_TAP_MIDDLE - audioDatas.balance);
	} else {
		return(audioDatas.balance - BAL_TAP_MIDDLE);
	}
}

slider_t getBalanceSlider() {
	if (audioDatas.balance == BAL_TAP_MIDDLE) {
		return(MIDDLE);
	} else if (audioDatas.balance < BAL_TAP_MIDDLE) {
		return(LEFT);
	} else {
		return(RIGHT);
	}
}

void displaySource() {
	switch(getLcdState()) {
		case BASIC:
			lcd.setCursor(11, 0);
			lcd.print(LCD_AFF_SRC[getSource()]);
			break;
		case LARGE:
			displayBasicInfos();
			break;
		case MENU:
			break;
	}
}

void displayLarge(byte number) {
	int d, u;
	if (number > 9) {
		d = (number - (number % 10)) / 10;
		u = number % 10;
	} else {
		d = 0;
		u = number;
	}
	printBigNum(d, 5, 0);
	printBigNum(u, 9, 0);
	setLcdState(LARGE);
}

dspfmt_t getLcdState() {
	return lcdState;
}

void setLcdState(dspfmt_t format) {
	lcdState = format;

	lcdLightOn(); // allume l'écran pour une période déterminé fonction du timestamp qui suit
	lastLcdDisplay = millis(); // dernier timestamp d'accès (quel que soit le menu)

	if (lcdState == LARGE) {
		lastLargeLcdDisplay = millis(); // dernier timestamp d'accès au menu LARGE
	}
}

void menuInit() {
	mm.add_menu(&mu1);
	mu1.add_item(&mu1_mi1, &onMenuSetVolMaxStart);
	//mu1.add_item(&mu1_mi2, &onMenuSetVolIncr);
	//mu1.add_item(&mu1_mi3, &onMenuSetBalIncr);

	mm.add_menu(&mu2);

	mu2.add_item(&mu2_mi01, &onMenuSetLedBrightness);
	mu2.add_item(&mu2_mi02, &onMenuSetLedBrightness);

	mu2.add_item(&mu2_mi30, &onMenuSetLedColorOn);
	mu2.add_item(&mu2_mi31, &onMenuSetLedColorOn);
	mu2.add_item(&mu2_mi32, &onMenuSetLedColorOn);
	mu2.add_item(&mu2_mi33, &onMenuSetLedColorOff);
	mu2.add_item(&mu2_mi34, &onMenuSetLedColorOff);
	mu2.add_item(&mu2_mi35, &onMenuSetLedColorOff);
	mu2.add_item(&mu2_mi36, &onMenuSetLedColorBlink);
	mu2.add_item(&mu2_mi37, &onMenuSetLedColorBlink);
	mu2.add_item(&mu2_mi38, &onMenuSetLedColorBlink);

	mu2.add_item(&mu2_mi50, &onMenuSetLedColorDefault);

	mm.add_menu(&mu3);

	mu3.add_item(&mu3_mi01, &onMenuSetLcdBackLightSaver);

	ms.set_root_menu(&mm);
}

/** Affiche le menu passé en argument **/
void menuHandler(cmd_t command) {
	if (getLcdState() == MENU) {
		switch(command) {
			case C_MENU_EXIT:
				audioDatas.neopxStatus = COLOR_ON;
				neopSetColor(audioDatas.neopxStatus);
				displayBasicInfos();
				break;
			case C_MENU_UP:
				ms.prev();
				menuLedStatusDependingOnContext(ms);
				displayMenu();
				break;
			case C_MENU_DOWN:
				ms.next();
				menuLedStatusDependingOnContext(ms);
				displayMenu();
				break;
			case C_MENU_LEFT:
				ms.back();
				menuLedStatusDependingOnContext(ms);
				displayMenu();
				break;
			case C_MENU_RIGHT:
				break;
			case C_MENU_OK:
				ms.select();
				displayMenu();
				break;
			default:
				break;
		}
	} else {
		if (command == C_MENU) {
			displayMenu();
		}
	}
}

void menuLedStatusDependingOnContext(MenuSystem ms) {
	audioDatas.neopxStatus = COLOR_ON;

	if (strcmp(ms.get_current_menu()->get_name(), MU2_LED) == 0) {
		if (strcmp(ms.get_current_menu()->get_selected()->get_name(), MI_COLOR_ON_R) == 0 || \
			strcmp(ms.get_current_menu()->get_selected()->get_name(), MI_COLOR_ON_G) == 0 || \
			strcmp(ms.get_current_menu()->get_selected()->get_name(), MI_COLOR_ON_B) == 0) {
			audioDatas.neopxStatus = COLOR_ON;
		} else if (strcmp(ms.get_current_menu()->get_selected()->get_name(), MI_COLOR_OFF_R) == 0 || \
				   strcmp(ms.get_current_menu()->get_selected()->get_name(), MI_COLOR_OFF_G) == 0 || \
				   strcmp(ms.get_current_menu()->get_selected()->get_name(), MI_COLOR_OFF_B) == 0) {
			audioDatas.neopxStatus = COLOR_OFF;
		} else if (strcmp(ms.get_current_menu()->get_selected()->get_name(), MI_COLOR_BLK_R) == 0 || \
				   strcmp(ms.get_current_menu()->get_selected()->get_name(), MI_COLOR_BLK_G) == 0 || \
				   strcmp(ms.get_current_menu()->get_selected()->get_name(), MI_COLOR_BLK_B) == 0) {
			audioDatas.neopxStatus = COLOR_BLINK;
		}
	}

	neopSetColor(audioDatas.neopxStatus);
}

void displayMenu() {
	// Display the menu
	const Menu *cp_menu = ms.get_current_menu();
	char* itemName = (char*)cp_menu->get_selected()->get_name();
	char data[] = "       ";
	char mu_path[16] = "";
	strcat(mu_path, cp_menu->get_name());
	strcat(mu_path, "/");
	strcat(mu_path, itemName);

	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(mu_path);   // lcd.print(String(cp_menu->get_name()) + "/" + itemName);

	if (strcmp(itemName, MI_VOLSMAX) == 0) {
		sprintf(data, "%d", audioDatas.volumeMaxStart);
	/*} else if (strcmp(itemName, MI_VOLSTEP) == 0) {
		sprintf(data, "%d", audioDatas.volumeStepDefault);
	} else if (strcmp(itemName, MI_BALSTEP) == 0) {
		sprintf(data, "%d", audioDatas.balanceStepDefault);*/
	} else if (strcmp(itemName, MI_BRIGHTP) == 0 || strcmp(itemName, MI_BRIGHTM) == 0) {
		sprintf(data, "%d", audioDatas.neopxBrightness);
	} else if (strcmp(itemName, MI_COLOR_ON_R) == 0 || strcmp(itemName, MI_COLOR_ON_G) == 0 || strcmp(itemName, MI_COLOR_ON_B) == 0) {
		sprintf(data, "#%02X%02X%02X", R_COLOR(audioDatas.neopxOn), G_COLOR(audioDatas.neopxOn), B_COLOR(audioDatas.neopxOn));
	} else if (strcmp(itemName, MI_COLOR_OFF_R) == 0 || strcmp(itemName, MI_COLOR_OFF_G) == 0 || strcmp(itemName, MI_COLOR_OFF_B) == 0) {
		sprintf(data, "#%02X%02X%02X", R_COLOR(audioDatas.neopxOff), G_COLOR(audioDatas.neopxOff), B_COLOR(audioDatas.neopxOff));
	} else if (strcmp(itemName, MI_COLOR_BLK_R) == 0 || strcmp(itemName, MI_COLOR_BLK_G) == 0 || strcmp(itemName, MI_COLOR_BLK_B) == 0) {
		sprintf(data, "#%02X%02X%02X", R_COLOR(audioDatas.neopxBlink), G_COLOR(audioDatas.neopxBlink), B_COLOR(audioDatas.neopxBlink));
	} else if (strcmp(itemName, MI_LCD_BL_SAVER) == 0) {
		sprintf(data, "%s", (audioDatas.lcdBackLightSaver == 1) ? "On" : "Off");
	}
	displayMenuData(data);
	setLcdState(MENU);
}

void displayMenuData(char* myData) {
	lcd.setCursor(4,1);
	lcd.print(F("       "));
	lcd.setCursor(4,1);
	lcd.print(myData);
}

void onMenuSetVolMaxStart(MenuItem* pMenuItem) {
	if (audioDatas.volumeMaxStart == VOL_TAP_MAX) {
		audioDatas.volumeMaxStart = 0;
	} else {
		audioDatas.volumeMaxStart++;
	}
}

/*void onMenuSetVolIncr(MenuItem* pMenuItem) {
	if (audioDatas.volumeStepDefault == VOL_STEP_DEFAULT_MAX) {
		audioDatas.volumeStepDefault = VOL_STEP_DEFAULT_MIN;
	} else {
		audioDatas.volumeStepDefault++;
	}
}

void onMenuSetBalIncr(MenuItem* pMenuItem) {
	if (audioDatas.balanceStepDefault == BAL_STEP_DEFAULT_MAX) {
		audioDatas.balanceStepDefault = BAL_STEP_DEFAULT_MIN;
	} else {
		audioDatas.balanceStepDefault++;
	}
}*/

void onMenuSetLedBrightness(MenuItem* pMenuItem) {
	if (strcmp(pMenuItem->get_name(), MI_BRIGHTM) == 0) {
		// décrémente
		if (audioDatas.neopxBrightness <= NEOPIXEL_BRIGHTNESS_MIN) {
			audioDatas.neopxBrightness = NEOPIXEL_BRIGHTNESS_MAX;
		} else if (audioDatas.neopxBrightness - NEOPIXEL_BRIGHTNESS_TAP < NEOPIXEL_BRIGHTNESS_MIN) {
			audioDatas.neopxBrightness = NEOPIXEL_BRIGHTNESS_MIN;
		} else {
			audioDatas.neopxBrightness -= NEOPIXEL_BRIGHTNESS_TAP;
		}
	} else {
		// incrémente
		if (audioDatas.neopxBrightness >= NEOPIXEL_BRIGHTNESS_MAX) {
			audioDatas.neopxBrightness = NEOPIXEL_BRIGHTNESS_MIN;
		} else if (audioDatas.neopxBrightness + NEOPIXEL_BRIGHTNESS_TAP > NEOPIXEL_BRIGHTNESS_MAX) {
			audioDatas.neopxBrightness = NEOPIXEL_BRIGHTNESS_MAX;
		} else {
			audioDatas.neopxBrightness += NEOPIXEL_BRIGHTNESS_TAP;
		}
	}
	neopSetBrightness();
}

void onMenuSetLedColorOn(MenuItem* pMenuItem) {
	audioDatas.neopxOnDefault = 0;
	if (strcmp(pMenuItem->get_name(), MI_COLOR_ON_R) == 0) {
		audioDatas.neopxOn = increaseColor(audioDatas.neopxOn, RED);
	} else if (strcmp(pMenuItem->get_name(), MI_COLOR_ON_G) == 0) {
		audioDatas.neopxOn = increaseColor(audioDatas.neopxOn, GREEN);
	} else if (strcmp(pMenuItem->get_name(), MI_COLOR_ON_B) == 0) {
		audioDatas.neopxOn = increaseColor(audioDatas.neopxOn, BLUE);
	}
	audioDatas.neopxStatus = COLOR_ON;
	neopSetColor(audioDatas.neopxStatus);
}

void onMenuSetLedColorOff(MenuItem* pMenuItem) {
	audioDatas.neopxOffDefault = 0;
	if (strcmp(pMenuItem->get_name(), MI_COLOR_OFF_R) == 0) {
		audioDatas.neopxOff = increaseColor(audioDatas.neopxOff, RED);
	} else if (strcmp(pMenuItem->get_name(), MI_COLOR_OFF_G) == 0) {
		audioDatas.neopxOff = increaseColor(audioDatas.neopxOff, GREEN);
	} else if (strcmp(pMenuItem->get_name(), MI_COLOR_OFF_B) == 0) {
		audioDatas.neopxOff = increaseColor(audioDatas.neopxOff, BLUE);
	}
	audioDatas.neopxStatus = COLOR_OFF;
	neopSetColor(audioDatas.neopxStatus);
}

void onMenuSetLedColorBlink(MenuItem* pMenuItem) {
	audioDatas.neopxBlinkDefault = 0;
	if (strcmp(pMenuItem->get_name(), MI_COLOR_BLK_R) == 0) {
		audioDatas.neopxBlink = increaseColor(audioDatas.neopxBlink, RED);
	} else if (strcmp(pMenuItem->get_name(), MI_COLOR_BLK_G) == 0) {
		audioDatas.neopxBlink = increaseColor(audioDatas.neopxBlink, GREEN);
	} else if (strcmp(pMenuItem->get_name(), MI_COLOR_BLK_B) == 0) {
		audioDatas.neopxBlink = increaseColor(audioDatas.neopxBlink, BLUE);
	}
	audioDatas.neopxStatus = COLOR_BLINK;
	neopSetColor(audioDatas.neopxStatus);
}

void onMenuSetLedColorDefault(MenuItem* pMenuItem) {
	setColorDefault();
	audioDatas.neopxStatus = COLOR_ON;
	neopSetColor(audioDatas.neopxStatus);
}

void onMenuSetLcdBackLightSaver(MenuItem* pMenuItem) {
	if (audioDatas.lcdBackLightSaver == 1) {
		audioDatas.lcdBackLightSaver = 0;
	} else {
		audioDatas.lcdBackLightSaver = 1;
	}
}

/*
* -----------------------------------------------------------------
* Print big number over 2 lines, 3 colums per half digit
* -----------------------------------------------------------------
*/
void printBigNum(int number, int startCol, int startRow) {
	// Position cursor to requested position (each char takes 3 cols plus a space col)
	lcd.setCursor(startCol, startRow);

	// Each number split over two lines, 3 chars per line. Retrieve character
	// from the main array to make working with it here a bit easier.
	uint8_t thisNumber[6];
	for (int cnt = 0; cnt < 6; cnt++) thisNumber[cnt] = bigNums[number][cnt];

	// First line (top half) of digit
	for (int cnt = 0; cnt < 3; cnt++) lcd.print((char)thisNumber[cnt]);

	// Now position cursor to next line at same start column for digit
	lcd.setCursor(startCol, startRow + 1);

	// 2nd line (bottom half)
	for (int cnt = 3; cnt < 6; cnt++) lcd.print((char)thisNumber[cnt]);
}
