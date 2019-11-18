#ifndef NEOLIGHTSCOLORS_H
#define NEOLIGHTSCOLORS_H

#include <stdint.h>
#include <pru_cfg.h>
#include "resource_table_empty.h"
#include "prugpio.h"

#endif

#define STR_LEN 24
#define NUM_LED_BITS 24 // these are colored bits to send.
#define NUM_LEDS 64 // in Neomatrix. but this can change for other Adafruit neo versions .
#define oneCyclesOn     700/5   // Stay on 700ns
#define oneCyclesOff    800/5
#define zeroCyclesOn    350/5
#define zeroCyclesOff   600/5
#define resetCycles     60000/5 // Must be at least 50u, use 60u
#define out 1       // Bit number to output one

#define TURN_ON_LED_NUM

#define P9_27 (1<<5)
#define P9_28 (1<<3)
uint32_t gpio = P9_27  ; 

volatile register unsigned int __R30; // output gpio register for prun=0
volatile register unsigned int __R31; // input gpio register for prun=0

uint32_t *gpio1 = (uint32_t *)GPIO1;

// FUNCTION DEFINITIONS 
void TurnAllBlue(void);
void TurnAllGreen(void);
void TurnAllRed(void);
void TurnLEDColors(void);
void TurnOnAllLeds(void);
void TurnOffAllLeds(void);

