#include "neoLightsColors.c"
// interact with this file
// with arguments as macros
// from Makefile.


// this modules accepts the message passed to prmsg_pru30 kernel module
// index =0 : 
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
        

        // these variables are shared between If and else statement
        // If statement : Get new information from Kernel Module
        // Else statement : Uses variables which stores last information.
        int index ; 
        char *ret; 
        uint32_t dominant_color; // single dominant color
        uint32_t top_left, top_right, bottom_left, bottom_right ;  // four region colors 
        
        int val1,val2,val3,val4,val5,val6,val7,val8; // audioVisualizer bar heights.(bar height is between 1 and 10).


        // this block is activated if new information has arrived. i.e 
        // i.e something is written to the /dev/prumsg_pru30
        if (__R31 & HOST_INT) {
            CT_INTC.SICR_bit.STS_CLR_IDX = FROM_ARM_HOST;
            while ( (pru_rpmsg_receive(&transport, &src, &dst, payload, &len) == PRU_RPMSG_SUCCESS) & 0xf) {
                
                // parsing code comes here. 
                // determines which mode is chosen
                index = atoi(payload); 
                
                // 4 regions color mode chosen.
                // format : 0 color1 color2 color3 color4
                if(index==0){
                    
                    ret = strchr(payload,' ');
                    top_left = strtol(&ret[1],NULL,0);
                    ret = strchr(&ret[1],' ');
                    top_right = strtol(&ret[1],NULL,0);
                    ret = strchr(&ret[1],' ');
                    bottom_left = strtol(&ret[1],NULL,0);
                    ret = strchr(&ret[1],' ');
                    bottom_right = strtol(&ret[1],NULL,0);

                    TurnOffAllLeds();
                    __delay_cycles(10000);
                    LightQuadrants(top_left,top_right,bottom_left,bottom_right);
                    __delay_cycles(10000);
                    TurnOffAllLeds();
                    
                }
                // 1 dominant color mode chosen.
                // format : 1 color
                else if(index==1){
                    
                    ret = strchr(payload,' ');
                    dominant_color = strtol(&ret[1],NULL,0);

                    TurnOffAllLeds();
                    __delay_cycles(10000000);
                    TurnAllCustomColor(dominant_color);
                    __delay_cycles(10000000);
                    //TurnOffAllLeds();
                }

                // audioVisualizer mode chosen
                // format : 2 val1 val2 val3 val4 val5 val6 val7 val8
                else if(index == 2){
                    ret = strchr(payload,' ');
                       
                    val1 = strtol(&ret[1],NULL,0);
                    ret = strchr(&ret[1],' ');
                    val2 = strtol(&ret[1],NULL,0);
                    ret = strchr(&ret[1],' ');
                    val3 = strtol(&ret[1],NULL,0);
                    ret = strchr(&ret[1],' ');
                    val4 = strtol(&ret[1],NULL,0); 
                    ret = strchr(&ret[1],' ');
                    val5 = strtol(&ret[1],NULL,0); 
                    ret = strchr(&ret[1],' ');
                    val6 = strtol(&ret[1],NULL,0); 
                    ret = strchr(&ret[1],' ');
                    val7 = strtol(&ret[1],NULL,0); 
                    ret = strchr(&ret[1],' ');
                    val8 = strtol(&ret[1],NULL,0); 
                    
                    int arr[8] = {val1,val2,val3,val4,val5,val6,val7,val8};
                    TurnOffAllLeds();
                    __delay_cycles(10000);
                    setLightBarsAll(arr);
                    __delay_cycles(10000);
                    TurnOffAllLeds();    
                
                }

                    __R30 &= ~(gpio);   
                    __delay_cycles(resetCycles);
            

                //__halt();
            }
        }

        // If nor information is received, keep doing stuff with the last information
        // this else statement ensures that PRU is doing something in between getting any 
        // updates from the neomatrix_interface
        else{

                
                // 4 regions color mode chosen.
                if(index==0){
                    
                    TurnOffAllLeds();
                    __delay_cycles(10000);
                    LightQuadrants(top_left,top_right,bottom_left,bottom_right);
                    __delay_cycles(10000);
                    TurnOffAllLeds();
                    
                }
                // 1 dominant color mode chosen.
                else if(index==1){

                    TurnOffAllLeds();
                    __delay_cycles(10000);
                    TurnAllCustomColor(dominant_color);
                    __delay_cycles(10000);
                    //TurnOffAllLeds();

                }

                // audioVisualization
                else if(index == 2){
                    
                    int myarr[8] = {val1,val2,val3,val4,val5,val6,val7,val8};
                    
                    TurnOffAllLeds();
                    __delay_cycles(10000);
                    setLightBarsAll(myarr);
                    __delay_cycles(10000);
                    TurnOffAllLeds();     
                    
                }
                    __R30 &= ~(gpio);   
                    __delay_cycles(resetCycles);
            

        }
    }
    

    // this block(3 lines) should come in the end of the pru proces
    // shutting down the PRU. 
}

// just for testing purposes.
// Turns on the Neomatrix with a custom color. specific_col
void test_specific_color(void){

    uint32_t specific_col = 0x0000ff;
                    TurnOffAllLeds();
                    __delay_cycles(10000);
                    TurnAllCustomColor(specific_col);
                    //__delay_cycles(100000000);
                    TurnOffAllLeds();

                    __R30 &= ~(gpio);   
                    __delay_cycles(resetCycles);
}

void main(void)
{   
    //test_specific_color();
    
    run_through_pmsg_pru();
    //test_setlightbar();
    //normal_lights();
    //test_specific_led();
    //test_light_quadrant();
}
// Turns off triggers
#pragma DATA_SECTION(init_pins, ".init_pins")
#pragma RETAIN(init_pins)
const char init_pins[] =  
	"/sys/class/leds/beaglebone:green:usr1/trigger\0none\0" \
	"/sys/class/leds/beaglebone:green:usr2/trigger\0none\0" \
	"\0\0";
