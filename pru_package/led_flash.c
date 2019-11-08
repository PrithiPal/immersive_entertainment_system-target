#include <stdint.h>
#include <pru_cfg.h>
#include "resource_table_empty.h"

volatile register uint32_t __R30 ; // output gpio register
volatile register uint32_t __R31 ; // input gpio register

void main(void){

    volatile uint32_t led, button; 
    led = 0x0020; 
    button = 0x0008 ; // not taking into account the button yet.
    int i = 0 ; 
    while(!(__R31 && button)){
        __R30 ^= led ; 
        __delay_cycles(500000000/100); 
        i=i+1 ; 
    }

    __halt() ; 
}

// Turns off triggers
#pragma DATA_SECTION(init_pins, ".init_pins")
#pragma RETAIN(init_pins)
const char init_pins[] =  
	"/sys/class/gpio/gpio115/direction\0out\0" \
    "/sys/class/gpio/gpio113/direction\0in\0" \
	"\0\0";
