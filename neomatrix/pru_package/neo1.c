// Control a ws2812 (NeoPixel) display, All on or all off
#include <stdint.h>
#include <pru_cfg.h>
#include "resource_table_empty.h"
#include "prugpio.h"

#define STR_LEN 24
#define oneCyclesOn     700/5   // Stay on 700ns
#define oneCyclesOff    800/5
#define zeroCyclesOn    350/5
#define zeroCyclesOff   600/5
#define resetCycles     60000/5 // Must be at least 50u, use 60u
#define out 1       // Bit number to output one

#define ONE

#define P9_27 (1<<5)
#define P9_28 (1<<3)
uint32_t gpio = P9_27 | P9_28 ; 

volatile register unsigned int __R30; // output gpio register for prun=0
volatile register unsigned int __R31; // input gpio register for prun=0

uint32_t *gpio1 = (uint32_t *)GPIO1;

void main(void)
{
    //Clear SYSCFG[STANDBY_INIT] to enable OCP master port /
    CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;

    uint32_t i;
    for(i=0; i<STR_LEN*2; i++) {
#ifdef ONE
        gpio1[GPIO_SETDATAOUT]   = USR2;
        __R30 |= gpio;      // Set the GPIO pin to 1
        __delay_cycles(oneCyclesOn-1);
        __R30 &= ~gpio;   // Clear the GPIO pin
        __delay_cycles(oneCyclesOff-2);
        gpio1[GPIO_CLEARDATAOUT] = USR2;
#else
        __R30 |= gpio;      // Set the GPIO pin to 1
        __delay_cycles(zeroCyclesOn-1);
        __R30 &= ~gpio;   // Clear the GPIO pin
        __delay_cycles(zeroCyclesOff-2);
#endif
    }
    // Send Reset
    __R30 &= ~gpio;   // Clear the GPIO pin
    __delay_cycles(resetCycles);

    __halt();
}

// Turns off triggers
#pragma DATA_SECTION(init_pins, ".init_pins")
#pragma RETAIN(init_pins)
const char init_pins[] =  
	"/sys/class/leds/beaglebone:green:usr1/trigger\0none\0" \
	"/sys/class/leds/beaglebone:green:usr2/trigger\0none\0" \
	"\0\0";
