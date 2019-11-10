#include "neoLightsColors.c"


// interact with this file
// with arguments as macros
// from Makefile.


void main(void)
{   

    CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;

    uint32_t color_palette[6]={
        COLOR1,
        COLOR2,
        COLOR3,
        COLOR4,
        COLOR5,
        COLOR6,
    };

    int i;
    for(i=0;i<6;i++){
        TurnOffAllLeds();
        __delay_cycles(100000000);
        TurnAllCustom(color_palette[i]);
        __delay_cycles(100000000);
    }

    /*
    int i;
    for(i=0;i<20;i++){
        TurnOffAllLeds();
        __delay_cycles(100000000);
        TurnAllRed();
        __delay_cycles(100000000);
        TurnAllBlue();
        __delay_cycles(100000000);
        TurnAllGreen();
        __delay_cycles(100000000);
        TurnOffAllLeds();
    }
    */
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
