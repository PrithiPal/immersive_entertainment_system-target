#include "neoLightsColors.c"
#include "prussdrv.h"
#include "pruss_intc_mapping.h"

// interact with this file
// with arguments as macros
// from Makefile.

void main(void)
{   

    CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;
    
    /*
    char buff[255];
    FILE *fp ; 
    fp = fopen("simple.txt","r");
    fscanf(fp,"%s",buff); 
    */

    uint32_t specific_color = buff;

    while(1){
        TurnOffAllLeds();
        __delay_cycles(100000000);
        moveLED(specific_color);
        __delay_cycles(100000000);
        TurnOffAllLeds();
    }

    // this block(3 lines) should come in the end of the pru proces
    // shutting down the PRU.
    
    
    __R30 &= ~(gpio);   // Clear the GPIO pin
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
