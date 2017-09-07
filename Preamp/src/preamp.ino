/*
  Preamplifier HiFi - IR (rc5), I2C (lcd, volume, balance)

  version   0.1
  created   15 Mar. 2017
  modified  20 Jul. 2017
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
	unsigned char ir_command;

	//DEBUG_PRINT(results.address);
	//DEBUG_PRINT(results.decode_type);  // UNKNOWN, NEC, SONY, RC5, ...
	//DEBUG_PRINT_HEX(results.value);
	//DEBUG_PRINT(results.bits);         // Number of bits in decoded value*/

	if (results.address == IRECV_ADDR && results.decode_type == RC6 /* 2 */ && results.bits == 20) {
		ir_toggle = bitRead(results.value, IRECV_TOGGLE_NUM_BIT);
		ir_command = results.value;

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
			case C_MUTE_ON:
				setMuteOn();
				break;
			case C_MUTE_OFF:
				setMuteOff();
				break;
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
			case C_SRC_6:
				setSource(S6);
				break;
			case C_SRC_7:
				setSource(S7);
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
	digitalWrite(PREAMPLIFIER_K_PIN, HIGH); // Step01 : switching on the pre-amplifier (all the supply voltage)
	delay(4);
	initI2c();
	delay(2);
	restoreAudioDatas();                    // Step02 : get previous state (before switching off)
	setSource(getSource(), true);           // Step03 : set source channel
#ifdef DS1882
	initConfRegister();
#endif
	setVolume();                            // Step04 : set volume + balance
	digitalWrite(AMPLIFIER_K_PIN, HIGH);    // Step05 : switching on the amplifier
	lcdInit();                              // Step06 : initialize lcd device
	lcdOn();                                // Step07 : display standard informations
	digitalWrite(NEOPIXEL_GND_PIN, HIGH);   // Step08 : connect LED K to GND
	neopInit();                             // Step09 : switch on the neopixel led
	neopSetColor(audioDatas.neopxStatus);
}

void setPowerOff() {
	pwrState = OFF;
	digitalWrite(AMPLIFIER_K_PIN, LOW);     // Step01 : switching off the amplifier
	neopSetColor(COLOR_OFF);                // Step02 : switch color off the neopixel led
	lcdOff();                               // Step03 : switching off the display
	saveAudioDatas();                       // Step04 : store current pre-amplifier states
	digitalWrite(PREAMPLIFIER_K_PIN, LOW);  // Step05 : switching off the pre-amplifier (all the supply voltage)
	digitalWrite(LATCH_595_PIN, LOW);       // Step06 : to avoid having a resident voltage influencing the +5V
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
