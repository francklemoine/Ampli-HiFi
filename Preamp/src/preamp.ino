/*
  Preamplifier HiFi - IR (rc6), I2C (lcd, volume, balance)

  version   0.1
  created   15 Mar. 2017
  modified  27 Sep. 2017
  by Franck LEMOINE
*/
#include "Arduino.h"
#include <avr/sleep.h>
#include <avr/power.h>
#include <Adafruit_NeoPixel.h>
#include <IRremote.h>
#include <Wire.h>
#include "preamp.h"
#include "audio.h"
#include "lcd.h"
#include "neopixel.h"
#include "sources.h"
#include "storage.h"


IRrecv irrecv(IRECV_PIN);
decode_results results;

Adafruit_NeoPixel neopx = Adafruit_NeoPixel(NEOPIXEL_NUM, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

audioDatas_t audioDatas;

unsigned long lastLcdDisplay;
unsigned long lastLargeLcdDisplay;
unsigned long lastAtollIrecv;

byte irrecvPrevCommand  = 255; // Commande (IR) précédente inexistante (1ère réception sera  donc différente)
byte irrecvPrevToggle   = 255; // Bit toggle (IR) précédent inexistant (1ère réception sera  donc différente)
state_t pwrState        = OFF;

volatile byte irrecvInt = 0;

ISR (PCINT0_vect) {
	PCICR = 0;  // cancel pin change interrupts
	irrecvInt = 1;
} // end of ISR (PCINT0_vect)

void setup() {
	initPins();
	DEBUG_SERIAL_INIT();
	irrecv.enableIRIn(); // Start the receiver
	menuInit();

	//lcdInit();   // A ne pas faire ici, l'écran lcd
	//neopInit();  //   et la led neopixel ne sont pas
	//neopOff();   //   encore alimentés
}

void loop() {
	static unsigned long lastActivity;

	if (getPower() == OFF && millis() - lastActivity >= NO_ACTIVITY_TIMEOUT) {
		DEBUG_PRINT("hibernation");
		neopOff();
		hibernate();
		lastActivity = millis();
		DEBUG_PRINT("active");
	}

	if (getLcdState() == LARGE && \
		getPower() == ON && \
		millis() - lastLargeLcdDisplay >= LARGE_LCD_DISPLAY_TIMEOUT)
		displayBasicInfos();

	if (getLcdState() == BASIC && \
		getLcdLightStatus() == 1 && \
		getPower() == ON && \
		audioDatas.lcdBackLightSaver == 1 && \
		millis() - lastLcdDisplay >= LCD_DISPLAY_TIMEOUT)
		lcdLightOff();

	if (irrecv.decode(&results)) {
		irrecvInt = 0;
		manageIr();
		irrecv.resume(); // Receive the next value
		lastActivity = millis();
	}

	//DEBUG_PRINT(lastActivity);
}

void hibernate() {
	Serial.flush();  // wait for Serial to finish outputting
	Serial.end();    // shut down Serial
	noInterrupts();  // timed sequence coming up

	// pin change interrupt masks
	PCMSK0 |= bit(PCINT0);      // pin 8

	PCIFR  |= bit(PCIF0);       // clear any outstanding interrupts (Need only PCIF0)
	PCICR  |= bit(PCIE0);       // enable pin change interrupts (Need only PICE0)

	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	//set_sleep_mode(SLEEP_MODE_STANDBY);
	sleep_enable();

	byte old_ADCSRA = ADCSRA;
	// disable ADC to save power
	ADCSRA = 0;

//	power_twi_disable();
//	// turn off I2C
//	TWCR &= ~(bit(TWEN) | bit(TWIE) | bit(TWEA));
//
//	// turn off I2C pull-ups
//	digitalWrite (A4, LOW); //SDA
//	digitalWrite (A5, LOW); //SCL

	power_all_disable(); // turn off various modules
	interrupts();

	sleep_cpu();

	// cancel sleep as a precaution
	sleep_disable();
	power_all_enable();         // enable modules again
	ADCSRA = old_ADCSRA;        // re-enable ADC conversion

	DEBUG_SERIAL_INIT();
}

cmd_t getCommandFromIr(byte ir_command) {
	for (byte i = 0; i < ARRAY_SIZE(IRECV_C); i++) {
		if (IRECV_C[i] == ir_command) return (cmd_t)i;
	}
	return C_MENU_OK; // bidon
}

void manageIr() {
	unsigned char ir_toggle;
	unsigned char ir_command = IRECV_UNDEF;

	DEBUG_PRINT(results.address);      // Unused by RC6 but used by PANASONIC
	DEBUG_PRINT(results.decode_type);  // UNKNOWN, NEC, SONY, RC5, ...
	DEBUG_PRINT_HEX(results.value);
	DEBUG_PRINT(results.bits);         // Number of bits in decoded value*/
	DEBUG_PRINT("");

	if (results.decode_type == RC6 /* 2 */ && results.bits == IRECV_BITS_NUMBER) {
		ir_toggle = bitRead(results.value, IRECV_TOGGLE_NUM_BIT);
		ir_command = results.value;
	} else if ((results.decode_type == UNKNOWN || results.decode_type == PANASONIC) && \
		      ((results.address == IRECV_ATOLL1_ADDR && results.bits == IRECV_ATOLL1_BITS_NUMBER) || \
			  (results.address == IRECV_ATOLL2_ADDR && results.bits == IRECV_ATOLL2_BITS_NUMBER))) {
		switch(results.value) {
			case IRECV_ATOLL1_PWR:
			case IRECV_ATOLL2_PWR:
				ir_command = IRECV_PWR;
				if (irrecvPrevCommand != IRECV_PWR || (irrecvPrevCommand == IRECV_PWR && millis() - lastAtollIrecv >= ATOLL_IRECV_TOGGLE_PWR_TIMEOUT)) {
					lastAtollIrecv = millis();
					ir_toggle = ~irrecvPrevToggle;
				} else {
					ir_toggle = irrecvPrevToggle;
				}
				break;
			case IRECV_ATOLL1_MUTE:
			case IRECV_ATOLL2_MUTE:
				ir_command = IRECV_MUTE;
				if (irrecvPrevCommand != IRECV_MUTE || (irrecvPrevCommand == IRECV_MUTE && millis() - lastAtollIrecv >= ATOLL_IRECV_TOGGLE_MUT_TIMEOUT)) {
					lastAtollIrecv = millis();
					ir_toggle = ~irrecvPrevToggle;
				} else {
					ir_toggle = irrecvPrevToggle;
				}
				break;
			case IRECV_ATOLL1_VOL_P:
			case IRECV_ATOLL2_VOL_P:
				ir_command = IRECV_VOL_P;
				break;
			case IRECV_ATOLL1_VOL_M:
			case IRECV_ATOLL2_VOL_M:
				ir_command = IRECV_VOL_M;
				break;
			case IRECV_ATOLL1_CD:
			case IRECV_ATOLL2_CD:
				ir_command = IRECV_1;
				break;
			case IRECV_ATOLL1_DVD:
			case IRECV_ATOLL2_DVD:
				ir_command = IRECV_2;
				break;
			case IRECV_ATOLL1_TUNER:
			case IRECV_ATOLL2_TUNER:
				ir_command = IRECV_3;
				break;
			case IRECV_ATOLL1_TAPE:
			case IRECV_ATOLL2_TAPE:
				ir_command = IRECV_4;
				break;
			case IRECV_ATOLL1_AUX:
			case IRECV_ATOLL2_AUX:
				ir_command = IRECV_5;
				break;
			case IRECV_ATOLL1_BY_PASS:
			case IRECV_ATOLL2_BY_PASS:
				ir_command = IRECV_BP;
				break;
			default:
				break;
		}
	} else {
		results.address = 0;
	}

	if (ir_command != IRECV_UNDEF) {
		//DEBUG_PRINT(ir_toggle);
		if (ir_command == IRECV_PWR) {
			if (irrecvPrevCommand == IRECV_PWR) {
				if (ir_toggle != irrecvPrevToggle) {
					irrecvPrevToggle = ir_toggle;
					if (getPower() == ON) {
						manageAudio(C_PWR_OFF);
					} else {
						manageAudio(C_PWR_ON);
					}
				}
			} else {
				irrecvPrevCommand = ir_command;
				irrecvPrevToggle  = ir_toggle;
				if (getPower() == ON) {
					manageAudio(C_PWR_OFF);
				} else {
					manageAudio(C_PWR_ON);
				}
			}
		} else if (getPower() == ON) {
			neopSetColor(COLOR_BLINK);
			if (ir_command == IRECV_MUTE) {
				if (irrecvPrevCommand == IRECV_MUTE) {
					if (ir_toggle != irrecvPrevToggle) {
						irrecvPrevToggle = ir_toggle;
						if (getMute() == ON) {
							manageAudio(C_MUTE_OFF);
						} else {
							manageAudio(C_MUTE_ON);
						}
					}
				} else {
					irrecvPrevCommand = ir_command;
					irrecvPrevToggle  = ir_toggle;
					if (getMute() == ON) {
						manageAudio(C_MUTE_OFF);
					} else {
						manageAudio(C_MUTE_ON);
					}
				}
			} else {
				irrecvPrevCommand = ir_command;
				manageAudio(getCommandFromIr(ir_command));
			}

			if (getLcdState() == MENU) {
				delay(50);
			} else {
				delay(25); // ne pas prendre en compte toutes les trames ir notamment avec les appuis long
			}
			neopSetColor(audioDatas.neopxStatus);
		}
	}
}

void manageAudio(cmd_t command) {
	//DEBUG_PRINT(command);
	if (command == C_PWR_ON) {
		setPowerOn();
	} else if (command == C_PWR_OFF) {
		setPowerOff();
	} else if (getLcdState() == MENU) {
		switch(command) {
			case C_MENU_UP:
			case C_MENU_DOWN:
			case C_MENU_LEFT:
			case C_MENU_RIGHT:
			case C_MENU_OK:
			case C_MENU_EXIT:
				menuHandler(command);
				break;
			default:
				break;
		}
	} else {
		switch(command) {
			case C_SRC_1:
				setSource(S1);
				break;
			case C_SRC_2:
				setSource(S2);
				break;
			case C_SRC_3:
				setSource(S3);
				break;
			case C_SRC_4:
				setSource(S4);
				break;
			case C_SRC_5:
				setSource(S5);
				break;
			case C_SRC_BP:
				setSource(BP);
				break;
			default:
				break;
		}
		if (getSource() != BP) {
			switch(command) {
				case C_MUTE_ON:
					setMuteOn();
					break;
				case C_MUTE_OFF:
					setMuteOff();
					break;
				case C_VOL_P:
					incVolume();
					break;
				case C_VOL_M:
					decVolume();
					break;
				case C_BAL_R:
					turnBalanceRight();
					break;
				case C_BAL_L:
					turnBalanceLeft();
					break;
				case C_MENU:
					menuHandler(command);
					break;
				case C_MENU_EXIT:
					if (getLcdState() == LARGE) displayBasicInfos();
					break;
				default:
					break;
			}
		}
	}
}

void initPins() {
	pinMode(LATCH_595_PIN,      OUTPUT); // set pins to output to control the shift register
	pinMode(CLOCK_595_PIN,      OUTPUT);
	pinMode(DATAS_595_PIN,      OUTPUT);

#ifdef USE_POTENTIOMETER_CE_PIN
	pinMode(POT_CE_PIN,         OUTPUT);
#endif

	pinMode(NEOPIXEL_GND_PIN,   OUTPUT);
	pinMode(AMPLIFIER_K_PIN,    OUTPUT);
	pinMode(PREAMPLIFIER_K_PIN, OUTPUT);
}

void initI2c() {
	Wire.begin(); // join i2c bus (address optional for master)
	//Wire.setClock(400000L);
	disableInternalPullupsOnSDAandSCL();
}

// retrieve current power state
state_t getPower() {
	return pwrState;
}

void setPowerOn() {
	pwrState = ON;
	restoreAudioDatas();                    // Step01 : get previous state (before switching off)
	digitalWrite(PREAMPLIFIER_K_PIN, HIGH); // Step02 : switching on the pre-amplifier (all the supply voltage)
	delay(6);
	initI2c();
	lcdInit();                              // Step03 : initialize lcd device / display basics informations
	setSource(getSource(), true);           // Step04 : set source channel
	#ifdef DS1882
		initConfRegister();
	#endif
	setVolume();                            // Step05 : set volume + balance
	digitalWrite(AMPLIFIER_K_PIN, HIGH);    // Step06 : switching on the amplifier
	digitalWrite(NEOPIXEL_GND_PIN, HIGH);   // Step07 : connect LED K to GND
	neopInit();                             // Step08 : switch on the neopixel led
	neopSetColor(audioDatas.neopxStatus);
}

void setPowerOff() {
	pwrState = OFF;
	saveAudioDatas();                       // Step01 : store current pre-amplifier states
	setMuteOn(false);                       // Step02 : mute
	digitalWrite(AMPLIFIER_K_PIN, LOW);     // Step03 : switching off the amplifier
	neopSetColor(COLOR_OFF);                // Step04 : switch color off the neopixel led
	lcdOff();                               // Step05 : switching off the display
	digitalWrite(PREAMPLIFIER_K_PIN, LOW);  // Step06 : switching off the pre-amplifier (all the supply voltage)
	digitalWrite(LATCH_595_PIN, LOW);       // Step07 : to avoid having a resident voltage influencing the +5V
	digitalWrite(CLOCK_595_PIN, LOW);       //           (remains a resident of light in the led)
	digitalWrite(DATAS_595_PIN, LOW);
	disableInternalPullupsOnSDAandSCL();
}

void disableInternalPullupsOnSDAandSCL() {
	// comming from : http://www.varesano.net/blog/fabio/how-disable-internal-arduino-atmega-pullups-sda-and-scl-i2c-bus
//#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega8__) || defined(__AVR_ATmega328P__)
	// deactivate internal pull-ups for twi
	// as per note from atmega8 manual pg167
	cbi(PORTC, 4);
	cbi(PORTC, 5);
//#else
	// deactivate internal pull-ups for twi
	// as per note from atmega128 manual pg204
//	cbi(PORTD, 0);
//	cbi(PORTD, 1);
//#endif
}
