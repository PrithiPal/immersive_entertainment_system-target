#include "neoLightsColors.c"
// interact with this file
// with arguments as macros
// from Makefile.



void run_through_pmsg_pru(void){

    CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;
    
    struct pru_rpmsg_transport transport;
	uint16_t src, dst, len;
	volatile uint8_t *status;
    uint32_t specific_color = 0xffffff ;

    CT_INTC.SICR_bit.STS_CLR_IDX = FROM_ARM_HOST;
    status = &resourceTable.rpmsg_vdev.status;
	while (!(*status & VIRTIO_CONFIG_S_DRIVER_OK));
    pru_rpmsg_init(&transport, &resourceTable.rpmsg_vring0, &resourceTable.rpmsg_vring1, TO_ARM_HOST, FROM_ARM_HOST);
    while (pru_rpmsg_channel(RPMSG_NS_CREATE, &transport, CHAN_NAME, CHAN_DESC, CHAN_PORT) != PRU_RPMSG_SUCCESS);
    while(1){


        // this block is activated if new information has arrived. i.e 
        // i.e something is written to the /dev/prumsg_pru30
        if (__R31 & HOST_INT) {
            CT_INTC.SICR_bit.STS_CLR_IDX = FROM_ARM_HOST;
            while ( (pru_rpmsg_receive(&transport, &src, &dst, payload, &len) == PRU_RPMSG_SUCCESS) & 0xf) {
                
                // parsing code comes here. 
                char *ret;
               
                ret = strchr(payload,' ');
                specific_color = strtol(&ret[0],NULL,0) ; 

                // actual light code here.
                TurnOffAllLeds();
                __delay_cycles(10000000);
                moveLED(specific_color);
                __delay_cycles(10000000);
                TurnOffAllLeds();

                __R30 &= ~(gpio);   // Clear the GPIO pin
                __delay_cycles(resetCycles);
                //__halt();
            }
        }

        // If nor information is received, keep doing stuff with the last information(specific_color)
        else{
                TurnOffAllLeds();
                __delay_cycles(10000000);
                moveLED(specific_color);
                __delay_cycles(10000000);
                TurnOffAllLeds();

        }
    }
    

    // this block(3 lines) should come in the end of the pru proces
    // shutting down the PRU. 
}

void normal_lights(void){
    
    CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;

    uint32_t specific_color = 0x00ffff ;

    TurnOffAllLeds();
    __delay_cycles(100000000);
    moveLED(specific_color);
    __delay_cycles(100000000);
    TurnOffAllLeds();

    __R30 &= ~(gpio);   // Clear the GPIO pin
    __delay_cycles(resetCycles);
    __halt();


}

void main(void)
{   
    
    run_through_pmsg_pru();
    //normal_lights();
    
}
// Turns off triggers
#pragma DATA_SECTION(init_pins, ".init_pins")
#pragma RETAIN(init_pins)
const char init_pins[] =  
	"/sys/class/leds/beaglebone:green:usr1/trigger\0none\0" \
	"/sys/class/leds/beaglebone:green:usr2/trigger\0none\0" \
	"\0\0";
