#ifndef PREAMP_H
#define PREAMP_H

#include "Arduino.h"

//#define MYDEBUG   //enable/disable serial debug output

#ifdef MYDEBUG
	#define DEBUG_SERIAL_INIT() Serial.begin(9600);
	#define DEBUG_PRINT(x)                 \
		Serial.print(millis());            \
		Serial.print(": ");                \
		Serial.print(__PRETTY_FUNCTION__); \
		Serial.print(' ');                 \
		Serial.print(__FILE__);            \
		Serial.print(':');                 \
		Serial.print(__LINE__);            \
		Serial.print(' ');                 \
		Serial.println(x);
	#define DEBUG_PRINT_HEX(x)             \
		Serial.print(millis());            \
		Serial.print(": ");                \
		Serial.print(__PRETTY_FUNCTION__); \
		Serial.print(' ');                 \
		Serial.print(__FILE__);            \
		Serial.print(':');                 \
		Serial.print(__LINE__);            \
		Serial.print(' ');                 \
		Serial.println(x, HEX);
#else
	#define DEBUG_SERIAL_INIT()
	#define DEBUG_PRINT(x)
	#define DEBUG_PRINT_HEX(x)
#endif

#define B_COLOR(w) ((uint8_t) ((w) & 0xff))
#define G_COLOR(w) ((uint8_t) ((w) >> 8))
#define R_COLOR(w) ((uint8_t) ((w) >> 16))

typedef enum color {
	COLOR_ON,
	COLOR_OFF,
	COLOR_OFF_0,
	COLOR_BLINK
} color_t;

typedef enum basecolor {
	RED,
	GREEN,
	BLUE
} basecolor_t;

typedef enum slider {
	MIDDLE,
	RIGHT,
	LEFT
} slider_t;

typedef enum dspfmt {
	BASIC,
	LARGE,
	MENU
} dspfmt_t;

typedef struct myAudioDatas {
	byte volume;
	byte mute;
	byte balance;
	byte source; // S1(MSB), S2, S3, S4, S5, S6, S7, HP(LSB)
	byte volumeMaxStart;
	//byte volumeStepDefault;
	//byte balanceStepDefault;

	byte neopxBrightness;
	color_t neopxStatus;
	byte neopxOnDefault; // 0 quand valeur personnalisée de la led allumée
	unsigned long neopxOn;
	byte neopxOffDefault; // 0 quand valeur personnalisée de la led éteinte
	unsigned long neopxOff;
	byte neopxBlinkDefault; // 0 quand valeur personnalisée de la led en mode clignotement
	long neopxBlink;

	byte lcdBackLightSaver;
} audioDatas_t;

#define ARRAY_SIZE(array)   (sizeof(array) / sizeof(*array))

const unsigned long NO_ACTIVITY_TIMEOUT       = 60000; // milliseconds
const unsigned long LCD_DISPLAY_TIMEOUT       = 60000; // milliseconds
const unsigned long LARGE_LCD_DISPLAY_TIMEOUT = 5000;  // milliseconds

typedef enum state {
	OFF,
	ON
} state_t;

const int  ADDR_MEM_BASE         = 0;  // Arduino Mega (ATmega2560) = 4kb from 0 to 4095 / Arduino Nano (ATmega328) = 1kb
const int  LATCH_595_PIN         = A1; // Connected to ST_CP of 74HC595  -  ATMEGA328-pin24 (PC1, PCINT9)
const int  CLOCK_595_PIN         = A2; // Connected to SH_CP of 74HC595  -  ATMEGA328-pin25 (PC2, PCINT10)
const int  DATAS_595_PIN         = A0; // Connected to DS of 74HC595     -  ATMEGA328-pin23 (PC0, PCINT8)

const byte VOL_STEP_DEFAULT_MIN  = 1;
const byte VOL_STEP_DEFAULT      = 1;
const byte VOL_STEP_DEFAULT_MAX  = 4;

const byte VOL_TAP_MAX           = 33; // 33 (90dB) <= volume <= 0 (OdB)
const byte VOL_TAP_STARTUP       = 25; // attenuation = 39db
const byte VOL_ADDR_W            = B00101000; //B01010000 - need to shift right (cf. Wire Library)
const byte BAR_ADDR_W            = B00111100; //B01111000
const byte BAL_ADDR_W            = B00111101; //B01111000
const byte BAL_TAP_MIDDLE        = 10; // potentiometer middle tap value
const byte BAL_TAP_MAX           = 20;
const byte BAL_STEP_DEFAULT_MIN  = 1;
const byte BAL_STEP_DEFAULT      = 1;
const byte BAL_STEP_DEFAULT_MAX  = 10;

/*
* Tableau associant la valeur du volume avec son attenuation en décibels
* Ex : à un volume à 0 correspond une attenuation de 90db
*/
const byte vol2db[34] = {90, 60, 57, 54, 51, 48, 45, 42, 39, 36, 34, 32, 30, 28, 26, 24, 22, 20, 18, 16, 14, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

const int LCD_I2C_ADDR  = 0x3F;

// Listes des commandes
typedef enum sources {
	S_UNDEF, S1, S2, S3, S4, S5, S6, S7, HP
} sources_t;

// Remote Control
const byte IRECV_PIN               = 8;  // Connected to IR Receiver out pin (Atmega Pin12 = PB0)
const byte IRECV_ADDR              = 0;
const byte IRECV_TOGGLE_NUM_BIT    = 16;
const byte IRECV_PWR               = 0x0C;
const byte IRECV_MUTE              = 0x0D;
const byte IRECV_VOL_P             = 0x10;
const byte IRECV_VOL_M             = 0x11;
const byte IRECV_BAL_R             = 0x20; //0x28;
const byte IRECV_BAL_L             = 0x21; //0x2B;
const byte IRECV_1                 = 1;
const byte IRECV_2                 = 2;
const byte IRECV_3                 = 3;
const byte IRECV_4                 = 4;
const byte IRECV_5                 = 5;
const byte IRECV_6                 = 6;
const byte IRECV_7                 = 7;
const byte IRECV_MENU              = 0x54;
const byte IRECV_MENU_UP           = 0x58;
const byte IRECV_MENU_DOWN         = 0x59;
const byte IRECV_MENU_LEFT         = 0x5A;
const byte IRECV_MENU_RIGHT        = 0x5B;
const byte IRECV_MENU_OK           = 0x5C;
const byte IRECV_MENU_EXIT         = 0x31;

// Listes des commandes
typedef enum commands {
	C_PWR_ON,		// Power ON
	C_PWR_OFF,		// Power OFF
	C_MUTE_ON,		// Mute ON
	C_MUTE_OFF,		// Mute OFF
	C_VOL_P,		// Volume +
	C_VOL_M,		// Volume -
	C_BAL_R,		// Balance Droite
	C_BAL_L,		// Balance Gauche
	C_SRC_1,		// Source 1
	C_SRC_2,		// Source 2
	C_SRC_3,		// Source 3
	C_SRC_4,		// Source 4
	C_SRC_5,		// Source 5
	C_SRC_6,		// Source 6
	C_SRC_7,		// Source 7
	C_MENU,			// Accès au menu
	C_MENU_UP,		// Accès au menu
	C_MENU_DOWN,	// Accès au menu
	C_MENU_LEFT,	// Accès au menu
	C_MENU_RIGHT,	// Accès au menu
	C_MENU_OK,		// Accès au menu
	C_MENU_EXIT,	// Accès au menu
	C_COUNT
} cmd_t;

const byte IRECV_C[C_COUNT] = {
	[C_PWR_ON]     = IRECV_PWR,
	[C_PWR_OFF]    = IRECV_PWR,
	[C_MUTE_ON]    = IRECV_MUTE,
	[C_MUTE_OFF]   = IRECV_MUTE,
	[C_VOL_P]      = IRECV_VOL_P,
	[C_VOL_M]      = IRECV_VOL_M,
	[C_BAL_R]      = IRECV_BAL_R,
	[C_BAL_L]      = IRECV_BAL_L,
	[C_SRC_1]      = IRECV_1,
	[C_SRC_2]      = IRECV_2,
	[C_SRC_3]      = IRECV_3,
	[C_SRC_4]      = IRECV_4,
	[C_SRC_5]      = IRECV_5,
	[C_SRC_6]      = IRECV_6,
	[C_SRC_7]      = IRECV_7,
	[C_MENU]       = IRECV_MENU,
	[C_MENU_UP]    = IRECV_MENU_UP,
	[C_MENU_DOWN]  = IRECV_MENU_DOWN,
	[C_MENU_LEFT]  = IRECV_MENU_LEFT,
	[C_MENU_RIGHT] = IRECV_MENU_RIGHT,
	[C_MENU_OK]    = IRECV_MENU_OK,
	[C_MENU_EXIT]  = IRECV_MENU_EXIT
};

const byte AMPLIFIER_K_PIN         = A3;  // ATMEGA328-pin26 (PC3, PCINT11)
const byte PREAMPLIFIER_K_PIN      = 2;   // ATMEGA328-pin32 (PD2, PCINT18)

// LED NeoPixel
const byte NEOPIXEL_PIN            = 10;  // ATMEGA328-pin14 (PB2, PCINT2)
const byte NEOPIXEL_GND_PIN        = 3;   // ATMEGA328-pin1  (PD3, PCINT19)
const byte NEOPIXEL_NUM            = 1;   // only one led
const byte NEOPIXEL_BRIGHTNESS     = 100;
const byte NEOPIXEL_BRIGHTNESS_MAX = 250;
const byte NEOPIXEL_BRIGHTNESS_MIN = 10;
const byte NEOPIXEL_BRIGHTNESS_TAP = 10;
const byte NEOPIXEL_ON_R           = 0;
const byte NEOPIXEL_ON_G           = 0;
const byte NEOPIXEL_ON_B           = 100;
const byte NEOPIXEL_BLINK_R        = 100;
const byte NEOPIXEL_BLINK_G        = 0;
const byte NEOPIXEL_BLINK_B        = 0;
const byte NEOPIXEL_OFF_R          = 100;
const byte NEOPIXEL_OFF_G          = 0;
const byte NEOPIXEL_OFF_B          = 0;


/*
Code Télécommande Philips RC5410
================================
PP		(80)E
Mute	(80)D
Power	(80)C
1		(80)1
2		(80)2
3		(80)3
4		(80)4
5		(80)5
6		(80)6
7		(80)7
8		(80)8
9		(80)9
0		(80)0
<->		(8)38
-/--	(80)A
V+		(8)10
V-		(8)11
Lum+	(8)12
Lum-	(8)13
Con+	(8)14
Con-	(8)15
<<		(8)32
>>		(8)34
REC		(8)37
PAUSE	(8)36
PLAY	(8)35



Code Télécommande Universelle = TV - 0037 - Type = RC5
======================================================
1		(80)1
2		(80)2
3		(80)3
4		(80)4
5		(80)5
6		(80)6
7		(80)7
8		(80)8
9		(80)9
0		(80)0

Mute	(80)D
V+		(8)10
V-		(8)11
Menu	(8)3B
OK		(8)25
Bas		(8)21
Haut	(8)20
Gauche	(8)11
Droite	(8)10
i		(80)F

retour 	(8)22
AV		(8)38
Plein écran (en bas à gauche) (8)3C
Plein écran (en bas au milieu) (8)3F
Audio 	(8)23



Code Télécommande Universelle = TV - 1744 - Type = RC6
======================================================
1		(1000)1
2		(1000)2
3		(1000)3
4		(1000)4
5		(1000)5
6		(1000)6
7		(1000)7
8		(1000)8
9		(1000)9
0		(1000)0

Power	(1000)C
Mute	(1000)D
V+		(100)10
V-		(100)11
+		(100)20
-		(100)21
Menu	(100)54
Exit	(100)31
OK		(100)5C
Bas		(100)59
Haut	(100)58
Gauche	(100)5A
Droite	(100)5B
i		(1000)F
Guide	(100)CC

<<		(100)2B
>>		(100)28
>		(100)C1
||		(100)C1
Stop	(100)31 (idem Exit)
Rec		(100)37


retour 	(8)22
AV		(8)38
Plein écran (en bas à gauche) (8)3C
Plein écran (en bas au milieu) (8)3F
Audio 	(8)23
*/

/*
VOLUME	TAP POSITION	ATTENUATION (dB)
  33         0               0
  32         1               1
  31         2               2
  30         3               3
  29         4               4
  28         5               5
  27         6               6
  26         7               7
  25         8               8
  24         9               9
  23         10             10
  22         11             11
  21         12             12
  20         13             14
  19         14             16
  18         15             18
  17         16             20
  16         17             22
  15         18             24
  14         19             26
  13         20             28
  12         21             30
  11         22             32
  10         23             34
  9          24             36
  8          25             39
  7          26             42
  6          27             45
  5          28             48
  4          29             51
  3          30             54
  2          31             57
  1          32             60
  0          33             90
*/

#endif
