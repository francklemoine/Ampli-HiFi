EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:flem
LIBS:ampli_hifi-cache
EELAYER 25 0
EELAYER END
$Descr A3 16535 11693
encoding utf-8
Sheet 1 6
Title "Préamplificateur HiFi"
Date "2017-01-14"
Rev "1.0"
Comp "Franck LEMOINE"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L LCD16X2 DS101
U 1 1 587A6D65
P 10100 2350
F 0 "DS101" H 9300 2750 50  0000 C CNN
F 1 "LCD16X2" H 10800 2750 50  0000 C CNN
F 2 "WC1602A" H 10100 2300 50  0000 C CIN
F 3 "" H 10100 2350 50  0000 C CNN
	1    10100 2350
	0    1    1    0   
$EndComp
$Sheet
S 8050 1500 1050 1700
U 587A6E2A
F0 "I2C to LCD Board 587A6E29" 60
F1 "i2c_to_lcd/file587A6E29.sch" 60
F2 "LCD_VSS" O R 9100 1600 60 
F3 "LCD_VDD" O R 9100 1700 60 
F4 "LCD_Vo" O R 9100 1800 60 
F5 "LCD_RS" O R 9100 1900 60 
F6 "LCD_RW" O R 9100 2000 60 
F7 "LCD_EN" O R 9100 2100 60 
F8 "LCD_D0" O R 9100 2200 60 
F9 "LCD_D1" O R 9100 2300 60 
F10 "LCD_D2" O R 9100 2400 60 
F11 "LCD_D3" O R 9100 2500 60 
F12 "LCD_D4" O R 9100 2600 60 
F13 "LCD_D5" O R 9100 2700 60 
F14 "LCD_D6" O R 9100 2800 60 
F15 "LCD_D7" O R 9100 2900 60 
F16 "LCD_A" O R 9100 3000 60 
F17 "LCD_K" O R 9100 3100 60 
F18 "VCC" I L 8050 1700 60 
F19 "GND" I L 8050 1800 60 
F20 "SDA" I L 8050 2800 60 
F21 "SCL" I L 8050 2900 60 
$EndSheet
$Comp
L GND #PWR102
U 1 1 587A7CB5
P 7600 1900
F 0 "#PWR102" H 7600 1650 50  0001 C CNN
F 1 "GND" H 7600 1750 50  0000 C CNN
F 2 "" H 7600 1900 50  0000 C CNN
F 3 "" H 7600 1900 50  0000 C CNN
	1    7600 1900
	1    0    0    -1  
$EndComp
$Sheet
S 2300 1450 1800 4250
U 587ABF08
F0 "MCU Board 587ABF07" 60
F1 "mcu_board/file587ABF07.sch" 60
F2 "Torique_115_Brown" O R 4100 1700 60 
F3 "Torique_0_Violet" O R 4100 2000 60 
F4 "Torique_115_Grey" O R 4100 2100 60 
F5 "Torique_0_Blue" O R 4100 2400 60 
F6 "Torique_0_Orange" I R 4100 2900 60 
F7 "Torique_0_Black_9_Yellow" I R 4100 3000 60 
F8 "Torique_9_Red" I R 4100 3100 60 
F9 "Phase" I L 2300 1700 60 
F10 "Neutre" I L 2300 1800 60 
F11 "Terre" I L 2300 1900 60 
F12 "SDA" O R 4100 3400 60 
F13 "SCL" O R 4100 3500 60 
F14 "SCLK" O R 4100 3800 60 
F15 "MOSI" O R 4100 3900 60 
F16 "MISO" O R 4100 4000 60 
F17 "SS1" O R 4100 4100 60 
F18 "+5V" O L 2300 2300 60 
F19 "+7.2V" O L 2300 3150 60 
F20 "-7.2V" O L 2300 3250 60 
F21 "GND" O L 2300 3650 60 
F22 "S1" O R 4100 4400 60 
F23 "S2" O R 4100 4500 60 
F24 "S3" O R 4100 4600 60 
F25 "S4" O R 4100 4700 60 
F26 "S5" O R 4100 4800 60 
F27 "S6" O R 4100 4900 60 
F28 "S7" O R 4100 5000 60 
F29 "HP_Phone" O R 4100 5100 60 
F30 "LED_PWR_A" O R 4100 5400 60 
F31 "LED_PWR_K" O R 4100 5500 60 
F32 "+5P" O L 2300 2650 60 
$EndSheet
$Comp
L +5V #PWR101
U 1 1 587AC2EE
P 7600 1600
F 0 "#PWR101" H 7600 1450 50  0001 C CNN
F 1 "+5V" H 7600 1740 50  0000 C CNN
F 2 "" H 7600 1600 50  0000 C CNN
F 3 "" H 7600 1600 50  0000 C CNN
	1    7600 1600
	1    0    0    -1  
$EndComp
$Comp
L TRANSFO_01 T101
U 1 1 587AC74C
P 4800 2050
F 0 "T101" H 4800 2550 70  0000 C CNN
F 1 "Torique 2x9v" H 4800 1550 70  0000 C CNN
F 2 "" H 4800 2050 60  0000 C CNN
F 3 "" H 4800 2050 60  0000 C CNN
	1    4800 2050
	1    0    0    -1  
$EndComp
$Comp
L CONN_1 Phase101
U 1 1 587AFEAB
P 1550 1700
F 0 "Phase101" H 1630 1700 40  0000 L CNN
F 1 "CONN_1" H 1550 1755 30  0001 C CNN
F 2 "" H 1550 1700 60  0000 C CNN
F 3 "" H 1550 1700 60  0000 C CNN
	1    1550 1700
	-1   0    0    1   
$EndComp
$Comp
L CONN_1 Neutre101
U 1 1 587AFFED
P 1550 1800
F 0 "Neutre101" H 1630 1800 40  0000 L CNN
F 1 "CONN_1" H 1550 1855 30  0001 C CNN
F 2 "" H 1550 1800 60  0000 C CNN
F 3 "" H 1550 1800 60  0000 C CNN
	1    1550 1800
	-1   0    0    1   
$EndComp
$Comp
L CONN_1 Terre101
U 1 1 587B00C1
P 1550 1900
F 0 "Terre101" H 1630 1900 40  0000 L CNN
F 1 "CONN_1" H 1550 1955 30  0001 C CNN
F 2 "" H 1550 1900 60  0000 C CNN
F 3 "" H 1550 1900 60  0000 C CNN
	1    1550 1900
	-1   0    0    1   
$EndComp
$Sheet
S 8050 3800 1050 1600
U 587B47ED
F0 "Sound Board 587B47EC" 60
F1 "sound_board/file587B47EC.sch" 60
F2 "SDA" I L 8050 4650 60 
F3 "SCL" I L 8050 4750 60 
F4 "SCLK" I L 8050 4950 60 
F5 "MOSI" I L 8050 5050 60 
F6 "MISO" I L 8050 5150 60 
F7 "SS1" I L 8050 5250 60 
F8 "VCC" I L 8050 4000 60 
F9 "GND" I L 8050 4100 60 
$EndSheet
Wire Wire Line
	9100 1600 9600 1600
Wire Wire Line
	9100 1700 9600 1700
Wire Wire Line
	9100 1800 9600 1800
Wire Wire Line
	9100 1900 9600 1900
Wire Wire Line
	9100 2100 9600 2100
Wire Wire Line
	9100 2000 9600 2000
Wire Wire Line
	9100 2200 9600 2200
Wire Wire Line
	9100 2300 9600 2300
Wire Wire Line
	9100 2400 9600 2400
Wire Wire Line
	9100 2500 9600 2500
Wire Wire Line
	9100 2600 9600 2600
Wire Wire Line
	9100 2700 9600 2700
Wire Wire Line
	9100 2800 9600 2800
Wire Wire Line
	9100 2900 9600 2900
Wire Wire Line
	9100 3000 9600 3000
Wire Wire Line
	9100 3100 9600 3100
Wire Wire Line
	7600 1600 7600 1700
Wire Wire Line
	7600 1700 8050 1700
Wire Wire Line
	8050 1800 7600 1800
Wire Wire Line
	7600 1800 7600 1900
Wire Wire Line
	4100 1700 4400 1700
Wire Wire Line
	4100 2000 4400 2000
Wire Wire Line
	4100 2100 4400 2100
Wire Wire Line
	4100 2400 4400 2400
Wire Wire Line
	5200 2450 5300 2450
Wire Wire Line
	5300 2450 5300 2900
Wire Wire Line
	5300 2900 4100 2900
Wire Wire Line
	5200 2150 5400 2150
Wire Wire Line
	5400 1950 5400 3000
Wire Wire Line
	5400 3000 4100 3000
Wire Wire Line
	5200 1950 5400 1950
Connection ~ 5400 2150
Wire Wire Line
	5200 1650 5500 1650
Wire Wire Line
	5500 1650 5500 3100
Wire Wire Line
	5500 3100 4100 3100
Wire Wire Line
	1700 1700 2300 1700
Wire Wire Line
	1700 1800 2300 1800
Wire Wire Line
	1700 1900 2300 1900
Wire Wire Line
	8050 2800 7200 2800
Wire Wire Line
	7200 2800 7200 4650
Wire Wire Line
	7200 3400 4100 3400
Wire Wire Line
	8050 2900 7300 2900
Wire Wire Line
	7300 2900 7300 4750
Wire Wire Line
	7300 3500 4100 3500
Wire Wire Line
	7300 4750 8050 4750
Connection ~ 7300 3500
Wire Wire Line
	7200 4650 8050 4650
Connection ~ 7200 3400
Wire Wire Line
	4100 3800 7100 3800
Wire Wire Line
	7100 3800 7100 4950
Wire Wire Line
	7100 4950 8050 4950
Wire Wire Line
	4100 3900 7000 3900
Wire Wire Line
	7000 3900 7000 5050
Wire Wire Line
	7000 5050 8050 5050
Wire Wire Line
	4100 4000 6900 4000
Wire Wire Line
	6900 4000 6900 5150
Wire Wire Line
	6900 5150 8050 5150
Wire Wire Line
	4100 4100 6800 4100
Wire Wire Line
	6800 4100 6800 5250
Wire Wire Line
	6800 5250 8050 5250
$Comp
L GND #PWR104
U 1 1 587BF598
P 7600 4200
F 0 "#PWR104" H 7600 3950 50  0001 C CNN
F 1 "GND" H 7600 4050 50  0000 C CNN
F 2 "" H 7600 4200 50  0000 C CNN
F 3 "" H 7600 4200 50  0000 C CNN
	1    7600 4200
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR103
U 1 1 587BF6AE
P 7600 3900
F 0 "#PWR103" H 7600 3750 50  0001 C CNN
F 1 "+5V" H 7600 4040 50  0000 C CNN
F 2 "" H 7600 3900 50  0000 C CNN
F 3 "" H 7600 3900 50  0000 C CNN
	1    7600 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	7600 3900 7600 4000
Wire Wire Line
	7600 4000 8050 4000
Wire Wire Line
	7600 4200 7600 4100
Wire Wire Line
	7600 4100 8050 4100
$Comp
L +5V #PWR?
U 1 1 587C77DD
P 1950 2200
F 0 "#PWR?" H 1950 2050 50  0001 C CNN
F 1 "+5V" H 1950 2340 50  0000 C CNN
F 2 "" H 1950 2200 50  0000 C CNN
F 3 "" H 1950 2200 50  0000 C CNN
	1    1950 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	1950 2200 1950 2300
Wire Wire Line
	1950 2300 2300 2300
$Comp
L +7.2V #PWR?
U 1 1 587C9812
P 1950 3050
F 0 "#PWR?" H 1950 3000 20  0001 C CNN
F 1 "+7.2V" H 1950 3150 30  0000 C CNN
F 2 "" H 1950 3050 60  0000 C CNN
F 3 "" H 1950 3050 60  0000 C CNN
	1    1950 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	1950 3050 1950 3150
Wire Wire Line
	1950 3150 2300 3150
$Comp
L -7.2V #PWR?
U 1 1 587C9A2C
P 1950 3350
F 0 "#PWR?" H 1950 3480 20  0001 C CNN
F 1 "-7.2V" H 1950 3450 30  0000 C CNN
F 2 "" H 1950 3350 60  0000 C CNN
F 3 "" H 1950 3350 60  0000 C CNN
	1    1950 3350
	-1   0    0    1   
$EndComp
Wire Wire Line
	1950 3350 1950 3250
Wire Wire Line
	1950 3250 2300 3250
$Comp
L GND #PWR?
U 1 1 587CA0ED
P 1950 3750
F 0 "#PWR?" H 1950 3500 50  0001 C CNN
F 1 "GND" H 1950 3600 50  0000 C CNN
F 2 "" H 1950 3750 50  0000 C CNN
F 3 "" H 1950 3750 50  0000 C CNN
	1    1950 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	2300 3650 1950 3650
Wire Wire Line
	1950 3650 1950 3750
$Sheet
S 8050 6000 1050 2000
U 587EB2EA
F0 "Select Board 587EB2E9" 60
F1 "select_board/file587EB2E9.sch" 60
F2 "+5V" I L 8050 6200 60 
F3 "GND" I L 8050 6300 60 
F4 "S1" I L 8050 6700 60 
F5 "S2" I L 8050 6800 60 
F6 "S3" I L 8050 6900 60 
F7 "S4" I L 8050 7000 60 
F8 "S5" I L 8050 7100 60 
F9 "S6" I L 8050 7200 60 
F10 "S7" I L 8050 7300 60 
F11 "HP_Phone" I L 8050 7400 60 
F12 "PRE_OUT_L" I L 8050 7500 60 
F13 "PRE_OUT_R" I L 8050 7600 60 
F14 "TAPE_OUT_L" I L 8050 7700 60 
F15 "TAPE_OUT_R" I L 8050 7800 60 
F16 "PRE_IN_L" O R 9100 6700 60 
F17 "PRE_IN_R" O R 9100 6800 60 
$EndSheet
$Comp
L +5V #PWR?
U 1 1 587F2B2D
P 7600 6100
F 0 "#PWR?" H 7600 5950 50  0001 C CNN
F 1 "+5V" H 7600 6240 50  0000 C CNN
F 2 "" H 7600 6100 50  0000 C CNN
F 3 "" H 7600 6100 50  0000 C CNN
	1    7600 6100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 587F2C67
P 7600 6400
F 0 "#PWR?" H 7600 6150 50  0001 C CNN
F 1 "GND" H 7600 6250 50  0000 C CNN
F 2 "" H 7600 6400 50  0000 C CNN
F 3 "" H 7600 6400 50  0000 C CNN
	1    7600 6400
	1    0    0    -1  
$EndComp
Wire Wire Line
	7600 6100 7600 6200
Wire Wire Line
	7600 6200 8050 6200
Wire Wire Line
	7600 6400 7600 6300
Wire Wire Line
	7600 6300 8050 6300
Text Label 9300 6700 0    60   ~ 0
PRE_IN_L
Text Label 9300 6800 0    60   ~ 0
PRE_IN_R
Wire Wire Line
	9100 6700 9300 6700
Wire Wire Line
	9100 6800 9300 6800
Text Label 7800 7500 2    60   ~ 0
PRE_OUT_L
Text Label 7800 7600 2    60   ~ 0
PRE_OUT_R
Text Label 7800 7700 2    60   ~ 0
TAPE_OUT_L
Text Label 7800 7800 2    60   ~ 0
TAPE_OUT_R
Wire Wire Line
	8050 7500 7800 7500
Wire Wire Line
	8050 7600 7800 7600
Wire Wire Line
	8050 7700 7800 7700
Wire Wire Line
	8050 7800 7800 7800
Wire Wire Line
	4100 4400 6700 4400
Wire Wire Line
	6700 4400 6700 6700
Wire Wire Line
	6700 6700 8050 6700
Wire Wire Line
	4100 4500 6600 4500
Wire Wire Line
	6600 4500 6600 6800
Wire Wire Line
	6600 6800 8050 6800
Wire Wire Line
	4100 4600 6500 4600
Wire Wire Line
	6500 4600 6500 6900
Wire Wire Line
	6500 6900 8050 6900
Wire Wire Line
	4100 4700 6400 4700
Wire Wire Line
	6400 4700 6400 7000
Wire Wire Line
	6400 7000 8050 7000
Wire Wire Line
	4100 4800 6300 4800
Wire Wire Line
	6300 4800 6300 7100
Wire Wire Line
	6300 7100 8050 7100
Wire Wire Line
	4100 4900 6200 4900
Wire Wire Line
	6200 4900 6200 7200
Wire Wire Line
	6200 7200 8050 7200
Wire Wire Line
	4100 5000 6100 5000
Wire Wire Line
	6100 5000 6100 7300
Wire Wire Line
	6100 7300 8050 7300
Wire Wire Line
	4100 5100 6000 5100
Wire Wire Line
	6000 5100 6000 7400
Wire Wire Line
	6000 7400 8050 7400
$Sheet
S 3200 9200 1600 1000
U 58818B37
F0 "On/Off Ampli Board 58818B36" 60
F1 "onoff_ampli_board/file58818B36.sch" 60
F2 "Phase_IN" I L 3200 9400 60 
F3 "Neutre_IN" I L 3200 9500 60 
F4 "Phase_OUT" O R 4800 9400 60 
F5 "Neutre_OUT" O R 4800 9500 60 
F6 "K_AlimAmpli" I L 3200 9800 60 
F7 "+5V" I L 3200 10000 60 
$EndSheet
$Comp
L DB9 J?
U 1 1 588196BC
P 2250 9600
F 0 "J?" H 2250 10150 50  0000 C CNN
F 1 "DB9" H 2250 9050 50  0000 C CNN
F 2 "" H 2250 9600 50  0000 C CNN
F 3 "" H 2250 9600 50  0000 C CNN
	1    2250 9600
	-1   0    0    -1  
$EndComp
Wire Wire Line
	2700 10000 3200 10000
Wire Wire Line
	2700 9800 3200 9800
$Comp
L LED D?
U 1 1 5882B044
P 4750 5400
F 0 "D?" H 4750 5550 50  0000 C CNN
F 1 "LED" H 4750 5300 50  0000 C CNN
F 2 "" H 4750 5400 50  0000 C CNN
F 3 "" H 4750 5400 50  0000 C CNN
	1    4750 5400
	-1   0    0    1   
$EndComp
Wire Wire Line
	4100 5400 4550 5400
Wire Wire Line
	4950 5400 4950 5400
Wire Wire Line
	4950 5400 4950 5500
Wire Wire Line
	4950 5500 4100 5500
$Comp
L APA-106 U?
U 1 1 58886548
P 4900 5950
F 0 "U?" H 4700 6250 60  0000 C CNN
F 1 "APA-106" H 4900 5650 60  0000 C CNN
F 2 "" H 4900 5500 60  0000 C CNN
F 3 "" H 4900 5500 60  0000 C CNN
	1    4900 5950
	1    0    0    -1  
$EndComp
$Comp
L +5P #PWR?
U 1 1 5888BA9D
P 1950 2550
F 0 "#PWR?" H 1950 2400 50  0001 C CNN
F 1 "+5P" H 1950 2690 50  0000 C CNN
F 2 "" H 1950 2550 50  0000 C CNN
F 3 "" H 1950 2550 50  0000 C CNN
	1    1950 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	1950 2550 1950 2650
Wire Wire Line
	1950 2650 2300 2650
$Comp
L GND #PWR?
U 1 1 5888CEC9
P 5400 6100
F 0 "#PWR?" H 5400 5850 50  0001 C CNN
F 1 "GND" H 5400 5950 50  0000 C CNN
F 2 "" H 5400 6100 50  0000 C CNN
F 3 "" H 5400 6100 50  0000 C CNN
	1    5400 6100
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5350 6100 5400 6100
NoConn ~ 5350 5800
$Sheet
S 2300 6200 1800 2200
U 588FD2C3
F0 "Power Board Sheet588FD2C2" 60
F1 "power_board/file588FD2C2.sch" 60
$EndSheet
$EndSCHEMATC
