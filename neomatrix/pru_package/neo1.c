#include "neoLightsColors.c"

void main(void)
{   

    CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;

    int i;
    for(i=0;i<5;i++){
        TurnOffAllLeds();
        __delay_cycles(10000000/10);
        TurnAllRed();
        __delay_cycles(1000000000/10);
        TurnAllBlue();
        __delay_cycles(1000000000/10);
        TurnAllGreen();
        __delay_cycles(1000000000/10);
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
