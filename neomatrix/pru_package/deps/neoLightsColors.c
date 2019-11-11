

#include <stdint.h>
#include <pru_cfg.h>
#include "resource_table_empty.h"
#include "prugpio.h"

// VARIABLE DEFINITIONS ----- 

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

// Pruout pins we want to interact with
// for testing purposes, you can join a simple bulb circuit to a spare pin
// to test whether PRU is operating when start or stop.
#define P9_27 (1<<5)
#define P9_28 (1<<3)
uint32_t gpio = P9_27 | P9_28 ;


volatile register unsigned int __R30; // output gpio register for prun=0
volatile register unsigned int __R31; // input gpio register for prun=0

uint32_t *gpio1 = (uint32_t *)GPIO1;


// FUNCTIONS DEFINITIONS ------ 

void bit_on(void){
    __R30 |= gpio;      // Set the GPIO pin to 1
    __delay_cycles(oneCyclesOn-1);
    __R30 &= ~(gpio);   // Clear the GPIO pin
    __delay_cycles(oneCyclesOff-2);
}
void bit_off(void){
    __R30 |= gpio;      // Set the GPIO pin to 1
    __delay_cycles(zeroCyclesOn-1);
    __R30 &= ~(gpio);   // Clear the GPIO pin
    __delay_cycles(zeroCyclesOff-2);
}

void TurnAllBlue(void){
 
    uint32_t custom_color = 0x000f0f; // blue color 
    uint32_t color[64] ;
    
    int i, j, k;
    for(k=0;k<64;k++){
        color[k] = custom_color;
    }

    
    for(j=0; j<64; j++) {
        for(i=23; i>=0; i--) {
            // logic to enable which leds to lit up.
            if(color[j] & (0x1<<i)) {
                bit_on();
            }
            else {
                bit_off();
            }

        }
    }
}

void TurnAllGreen(void){
 
    uint32_t custom_color = 0x0f0000; // blue color 
    uint32_t color[64] ;
    
    int i, j, k;
    for(k=0;k<64;k++){
        color[k] = custom_color;
    }

    
    for(j=0; j<64; j++) {
        for(i=23; i>=0; i--) {
            // logic to enable which leds to lit up.
            if(color[j] & (0x1<<i)) {
                bit_on();
            }
            else {
                bit_off();
            }

        }
    }
}

void TurnAllRed(void){
 
    uint32_t custom_color = 0x000f00; // blue color 
    uint32_t color[64] ;
    
    int i, j, k;
    for(k=0;k<64;k++){
        color[k] = custom_color;
    }    
    for(j=0; j<64; j++) {
        for(i=23; i>=0; i--) {
            // logic to enable which leds to lit up.
            if(color[j] & (0x1<<i)) {
                bit_on();
            }
            else {
                bit_off();
            }

        }
    }
}

// original source : https://markayoder.github.io/PRUCookbook/05blocks/blocks.html#_setting_neopixels_to_different_colors
void TurnLEDColors(void){
    
    #define STR_LEN2 3 
    uint32_t color[STR_LEN2] = {0x090000, 0x000900, 0x00009};    // green, red, blue
    int i, j;
    
    for(j=0; j<STR_LEN2; j++) {
        for(i=23; i>=0; i--) {
            // logic to enable which leds to lit up.
            if(color[j] & (0x1<<i)) {
                bit_on();
            } 
            else {
                bit_off();
            }

        }
    }
}


void TurnOnAllLeds(void){

    uint32_t i;
    for(i=0; i<NUM_LEDS*NUM_LED_BITS; i++) {

        __R30 |= gpio;      // Set the GPIO pin to 1
        __delay_cycles(oneCyclesOn-1);
        __R30 &= ~(gpio);   // Clear the GPIO pin
        __delay_cycles(oneCyclesOff-2);
    
    }
    
}
void TurnOffAllLeds(void){
    
    uint32_t i;
    for(i=0; i<64*24; i++) {
        __R30 |= gpio;      // Set the GPIO pin to 1
        __delay_cycles(zeroCyclesOn-1);
        __R30 &= ~(gpio);   // Clear the GPIO pin
        __delay_cycles(zeroCyclesOff-2);
    }
    
}

// pending : todo 
// function not exported to header file.
void StopForceNeoMatrix(void){
    // step 1 : turn all led off.
    // step 2 : stop the PRU unit running.
    TurnOffAllLeds();
}