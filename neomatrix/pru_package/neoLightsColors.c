#include <stdint.h>
#include <pru_cfg.h>
#include "resource_table_empty.h"
#include "prugpio.h"
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

void TurnAllGreen(void){

    
    uint32_t green_full_bright =  0x00000f ; 

    uint32_t i,j ; 
    for(i = 0 ; i< NUM_LEDS; i++){
        for(j = NUM_LED_BITS ; j >=0; j--)
        {
            if(green_full_bright & 0x1 << j){
                bit_on();
            }
            else{
                bit_off();
            }

        }

    }
}



// original source : https://markayoder.github.io/PRUCookbook/05blocks/blocks.html#_setting_neopixels_to_different_colors
void TurnLEDColors(void){
    

    #define STR_LEN2 3 
    uint32_t color[STR_LEN2] = {0xff0000, 0x000f00, 0x0000f};    // green, red, blue
    int i, j;

    for(j=0; j<STR_LEN2; j++) {
        for(i=23; i>=0; i--) {
            // logic to enable which leds to lit up.
            if(color[j] & (0x1<<i)) {
                bit_on();
            } else {
                bit_off();
            }

        }
    }

}

void TurnOnLed(void){
    
    uint32_t i;
    uint32_t led_num = 1*STR_LEN ; 
    
    for(i=0; i<STR_LEN*NUM_LEDS; i++) {
        
        __R30 |= gpio;      // Set the GPIO pin to 1
        __delay_cycles(oneCyclesOn-1);
        __R30 &= ~(gpio);   // Clear the GPIO pin
        __delay_cycles(oneCyclesOff-2);
    
        
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
    for(i=0; i<STR_LEN*NUM_LEDS; i++) {
        __R30 |= gpio;      // Set the GPIO pin to 1
        __delay_cycles(zeroCyclesOn-1);
        __R30 &= ~(gpio);   // Clear the GPIO pin
        __delay_cycles(zeroCyclesOff-2);
    }
    
}