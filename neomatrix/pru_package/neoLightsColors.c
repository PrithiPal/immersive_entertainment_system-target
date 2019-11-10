#include "neoLightsColors.h"


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
 
    uint32_t custom_color = 0x00000f; // blue color 
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
    for(i=0; i<STR_LEN*NUM_LEDS; i++) {

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