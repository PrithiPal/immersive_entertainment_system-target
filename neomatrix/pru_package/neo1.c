#include "neoLightsColors.c"
// interact with this file
// with arguments as macros
// from Makefile.



void run_through_pmsg_pru(void){

    CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;
    
    struct pru_rpmsg_transport transport;
	uint16_t src, dst, len;
	volatile uint8_t *status;
    //uint32_t specific_color = 0x000001 ;

    CT_INTC.SICR_bit.STS_CLR_IDX = FROM_ARM_HOST;
    status = &resourceTable.rpmsg_vdev.status;
	while (!(*status & VIRTIO_CONFIG_S_DRIVER_OK));
    pru_rpmsg_init(&transport, &resourceTable.rpmsg_vring0, &resourceTable.rpmsg_vring1, TO_ARM_HOST, FROM_ARM_HOST);
    while (pru_rpmsg_channel(RPMSG_NS_CREATE, &transport, CHAN_NAME, CHAN_DESC, CHAN_PORT) != PRU_RPMSG_SUCCESS);
    while(1){
        
        int index ; 
        char *ret; 
        uint32_t dominant_color;
        // this block is activated if new information has arrived. i.e 
        // i.e something is written to the /dev/prumsg_pru30
        if (__R31 & HOST_INT) {
            CT_INTC.SICR_bit.STS_CLR_IDX = FROM_ARM_HOST;
            while ( (pru_rpmsg_receive(&transport, &src, &dst, payload, &len) == PRU_RPMSG_SUCCESS) & 0xf) {
                
                // parsing code comes here. 
                index = atoi(payload); 
                
                // 4 regions color mode chosen.
                if(index==0){


                }
                // 1 dominant color mode chosen.
                else if(index==1){
                    uint32_t dominant_color;

                    ret = strchr(payload,' ');
                    dominant_color = strtol(&ret[1],NULL,0);
                    //dominant_color = 0x00000f;
                    //uint32_t dummy_color = 0x0000ff;         
                    TurnOffAllLeds();
                    __delay_cycles(1000000);
                    moveLED(dominant_color);
                    __delay_cycles(1000000);
                    TurnOffAllLeds();

                    __R30 &= ~(gpio);   
                    __delay_cycles(resetCycles);
                }
                
                else{
                    uint32_t dominant_color;

                    ret = strchr(payload,' ');
                    dominant_color = strtol(&ret[1],NULL,0);

                    //uint32_t dummy_color = 0x0000ff;         
                    TurnOffAllLeds();
                    __delay_cycles(1000000);
                    moveLED(0xffffff);
                    __delay_cycles(1000000);
                    TurnOffAllLeds();

                    __R30 &= ~(gpio);   
                    __delay_cycles(resetCycles);
                }
            
                //ret = strchr(payload,' ');
                //specific_color = strtol(&ret[0],NULL,0) ; 

                //__halt();
            }
        }

        // If nor information is received, keep doing stuff with the last information(specific_color)
        else{

                /*
                if(index==0){


                }
                // 1 dominant color mode chosen.
                else if(index==1){
                
                    //uint32_t dummy_color = 0x0000ff;         
                    TurnOffAllLeds();
                    __delay_cycles(10000000);
                    moveLED(dominant_color);
                    __delay_cycles(10000000);
                    TurnOffAllLeds();

                    __R30 &= ~(gpio);   
                    __delay_cycles(resetCycles);
                }

                */

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
