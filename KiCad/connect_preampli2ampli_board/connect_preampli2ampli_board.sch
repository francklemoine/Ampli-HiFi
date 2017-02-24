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
LIBS:connect_preampli2ampli_board-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Préamplificateur / Amplificateur HiFi"
Date "2017-02-16"
Rev "1.0"
Comp "Franck LEMOINE"
Comment1 "Carte de connexion à l'amplicateur (commande ON/OFF)"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L CONN_01X02 P101
U 1 1 58A58284
P 6300 2850
F 0 "P101" H 6300 3000 50  0000 C CNN
F 1 "CONN_01X02" H 6300 2700 50  0000 C CNN
F 2 "Measurement_Points:Test_Point_2Pads" H 6300 2850 50  0001 C CNN
F 3 "" H 6300 2850 50  0000 C CNN
	1    6300 2850
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X02 P102
U 1 1 58A5829B
P 6300 3700
F 0 "P102" H 6300 3850 50  0000 C CNN
F 1 "CONN_01X02" H 6300 3550 50  0000 C CNN
F 2 "Measurement_Points:Test_Point_2Pads" H 6300 3700 50  0001 C CNN
F 3 "" H 6300 3700 50  0000 C CNN
	1    6300 3700
	1    0    0    -1  
$EndComp
Text Label 5600 2900 2    60   ~ 0
K-AlimAmpli
Text Label 5600 3750 2    60   ~ 0
K-AlimAmpli
Wire Wire Line
	5600 2900 6100 2900
Wire Wire Line
	5600 3750 6100 3750
$Comp
L +5V #PWR01
U 1 1 58A58D70
P 6000 2600
F 0 "#PWR01" H 6000 2450 50  0001 C CNN
F 1 "+5V" H 6000 2740 50  0000 C CNN
F 2 "" H 6000 2600 50  0000 C CNN
F 3 "" H 6000 2600 50  0000 C CNN
	1    6000 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	6000 2600 6000 2800
Wire Wire Line
	6000 2800 6100 2800
$Comp
L +5V #PWR02
U 1 1 58A58D8E
P 6000 3450
F 0 "#PWR02" H 6000 3300 50  0001 C CNN
F 1 "+5V" H 6000 3590 50  0000 C CNN
F 2 "" H 6000 3450 50  0000 C CNN
F 3 "" H 6000 3450 50  0000 C CNN
	1    6000 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	6000 3450 6000 3650
Wire Wire Line
	6000 3650 6100 3650
Text Notes 6600 2850 0    60   ~ 0
Input (carte mcu préampli)
Text Notes 6600 3700 0    60   ~ 0
Output (ampli)
$Comp
L PWR_FLAG #FLG03
U 1 1 58A58DE8
P 5800 2550
F 0 "#FLG03" H 5800 2645 50  0001 C CNN
F 1 "PWR_FLAG" H 5800 2730 50  0001 C CNN
F 2 "" H 5800 2550 50  0000 C CNN
F 3 "" H 5800 2550 50  0000 C CNN
	1    5800 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 2550 5800 2700
Wire Wire Line
	5800 2700 6000 2700
Connection ~ 6000 2700
$EndSCHEMATC
