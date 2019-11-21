#include "neoLightsColors.h"


// FUNCTION DECLARATIONS ----------------------------------------
void bit_on(void);
void bit_off(void);
void setLightBar(int col_num,int height);
void normal_lights(void);
void moveLED(uint32_t specific_color);
void LightQuadrants(uint32_t color1, uint32_t color2, uint32_t color3, uint32_t color4);
void TurnOnSpecificLED(const int r,const int c,const uint32_t specific_color);
void TurnAllBlue(void);
void TurnAllCustomColor(uint32_t my_color);
void DisplayAllColors(void);
void DisplayColorSpectrum(void);
void TurnAllGreen(void);
void TurnAllRed(void); 
void TurnLEDColors(void); 
void TurnOffAllLeds(void); 
void TurnOnAllLeds(void); 
void StopForceNeoMatrix(void); 

//--------------------------------------------------------------------------------
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

// 1 <= col_num <= 8 and 8 <= height <= 1
void setLightBar(int col_num,int height){

    uint32_t color[NUM_LEDS] ;
    uint32_t specific_color = 0xffffff ; // later on make arg from function
    int i,j,k ; 
    int counter = 0 ; 
    uint8_t r,g,b ; 
    int height_counter = 8 ; 

    for(k=0;k<NUM_LEDS;k++){
        
        if( ( (k==0) || (k==8) || (k==16) || (k==24) || (k==32) || (k==40) || (k==48) || (k==56) )  && (col_num == 1)  ){
            
            r = 0x00;
            g = 0x00;
            b = 0xff;
            specific_color = g << 16 | r << 8 | b ; 

            if(height_counter<=height){
                color[k] = specific_color ; 
            }
            
            height_counter--;
        }
        else if( ( (k==1) || (k==9) || (k==17) || (k==25) || (k==33) || (k==41) || (k==49) || (k==57) ) && (col_num == 2)  ){
            
            r = 0x00;
            g = 0x00;
            b = 0xff;
            specific_color = g << 16 | r << 8 | b ;

            if(height_counter<=height){
                color[k] = specific_color ; 
            }
            
            height_counter--;
        }
        else if( ( (k==2) || (k==10) || (k==18) || (k==26) || (k==34) || (k==42) || (k==50) || (k==58)  ) && (col_num == 3) ){
            
            r = 0x00;
            g = 0x00;
            b = 0xff;
            specific_color = (g << 16) | (r << 8) | b ; 

            if(height_counter<=height){
                color[k] = specific_color ; 
            }
            
            height_counter--;
        }
        else if( ( (k==3) || (k==11) || (k==19) || (k==27) || (k==35) || (k==43) || (k==51) || (k==59) ) && (col_num == 4) ){
            
            r = 0x00;
            g = 0x00;
            b = 0xff;
            specific_color = (g << 16) | (r << 8) | b ; 

            if(height_counter<=height){
                color[k] = specific_color ; 
            }
            
            height_counter--;
        }
        else if( ( (k==4) || (k==12) || (k==20) || (k==28) || (k==36) || (k==44) || (k==52) || (k==60) )&& (col_num == 5) ){
           
            r = 0x00;
            g = 0xff;
            b = 0x00;
            specific_color = (g << 16) | (r << 8) | b ; 

            if(height_counter<=height){
                color[k] = specific_color ; 
            }
            
            height_counter--;
        }
        else if( ( (k==5) || (k==13) || (k==21) || (k==29) || (k==37) || (k==45) || (k==53) || (k==61) ) && (col_num == 6) ){
            
            r = 0x00;
            g = 0x00;
            b = 0xff;
            specific_color = (g << 16) | (r << 8) | b ; 

            if(height_counter<=height){
                color[k] = specific_color ; 
            }
            
            height_counter--;
        }
        else if( ( (k==6) || (k==14) || (k==22) || (k==30) || (k==38) || (k==46) || (k==54) || (k==62) ) && (col_num == 7) ){
            
            r = 0x00;
            g = 0x00;
            b = 0xff;        
            specific_color = (g << 16) | (r << 8) | b ; 

            if(height_counter<=height){
                color[k] = specific_color ; 
            }
            
            height_counter--;
        }
        else if( ( (k==7) || (k==15) || (k==23) || (k==31) || (k==39) || (k==47) || (k==55) || (k==63) ) && (col_num == 8) ){
            
            r = 0xff;
            g = 0x00;
            b = 0x00;

            specific_color = (g << 16) | (r << 8) | b ; 

            if(height_counter<=height){
                color[k] = specific_color ; 
            }
            
            height_counter--;
        }   
        else{
            color[k] = 0x000000 ; 
        }
    
    }


    for(j=0; j<=NUM_LEDS; j++) {
        for(i=NUM_LED_BITS-1; i>=0; i--) {
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

// assumes that 1 <= r,c <= 8
void moveLED(uint32_t specific_color){ 

    int led_pos = 64 ; 
    uint32_t color[NUM_LEDS] ;
    int i, j, k;
    // not an optimized method.

    for(k=0;k<=led_pos;k++){
        
        color[k] = specific_color; 
        
    }

    int l;
    // led number selection
    for(l=0;l<=64;l++){

        for(k=0;k<=l;k++){
            if(k==l){
                color[k] = specific_color; 
            }
            else{
                color[k] = 0x000000; 
            }
            
        } 
        
        // single led
        for(j=0; j<=l; j++) {
            // all 24 bits
            for(i=NUM_LED_BITS-1; i>=0; i--) {
                // logic to enable which leds to lit up.
                if(color[j] & (0x1<<i)) 
                {
                    bit_on();
                }
                else 
                {
                    bit_off();
                }
            }
        }
        __delay_cycles(10000000);
    }
}


void LightQuadrants(uint32_t color1, uint32_t color2, uint32_t color3, uint32_t color4){
    

    uint32_t color[NUM_LEDS] ;
    int i,j,k ; 
    int lower_or_upper = NUM_LEDS / 2 ; 
    for(k = 0 ; k < NUM_LEDS; k++){
        
        int k_mod = k % MAX_ROWS ; 
        // upper
        if(k < lower_or_upper){
            // top-left
            if(k_mod >=0 && k_mod <4){
                color[k] = color1 ; 
            }
            // top-right
            else if(k_mod >=4 && k_mod <8){
                color[k] = color2 ; 
            }
        }
        // lower
        else if(k >= lower_or_upper){

            // bottom-left
            if(k_mod >=0 && k_mod <4){
                color[k] = color3 ; 
            }
            // bottom-right
            else if(k_mod >=4 && k_mod <8){
                color[k] = color4 ; 
            }     
            
        }
        
    }

    for(j=0; j<NUM_LEDS; j++) {
        for(i=NUM_LED_BITS-1; i>=0; i--) {
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


void TurnOnSpecificLED(const int r,const int c,const uint32_t specific_color){
    int led_pos = 8*c + r ; 
    uint32_t color[NUM_LEDS] ;
    int i, j, k;
    // not an optimized method.

    for(k=0;k<=led_pos;k++){
        if(!(k-led_pos)){
            color[k] = specific_color; 
        }
        else
        {
            color[k] = 0x000000; // no color bit.
        }
        
    }

    for(j=0; j<=led_pos; j++) {
        for(i=NUM_LED_BITS-1; i>=0; i--) {
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


void TurnAllBlue(void){
    

    uint32_t custom_color = 0x020200; // blue color 
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

void TurnAllCustomColor(uint32_t my_color){
    
    uint32_t custom_color = my_color; // blue color 
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

void DisplayAllColors(void){

    int i;
    for(i=0;i<TOTAL_NUM_COLORS;i++){
        TurnOffAllLeds();
        __delay_cycles(100000);
        TurnAllCustomColor(color_palette[i]);
        //__delay_cycles(100000000/2);
    }

}
void DisplayColorSpectrum(void){

    int i;
    uint32_t color = 0x000001;
    for(i=0;i<256*256*256;i++){
        TurnOffAllLeds();
        __delay_cycles(1000000);
        TurnAllCustomColor(color);
        color = color + 1 ; 
        __delay_cycles(10000000);
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


void test_specific_led(void){
    uint32_t specific_color = 0xffffff; 

    TurnOffAllLeds();
    __delay_cycles(1000000);
    TurnOnSpecificLED(1,1,specific_color);
    __delay_cycles(1000000000);
    TurnOffAllLeds();  
}

void test_light_quadrant(void){
    uint32_t color1 = 0x110000 ; 
    uint32_t color2 = 0x001100 ; 
    uint32_t color3 = 0x000011 ; 
    uint32_t color4 = 0x111111 ; 

    TurnOffAllLeds();
    __delay_cycles(1000000);
    LightQuadrants(color1,color2,color3,color4);
    __delay_cycles(1000000000);
    TurnOffAllLeds(); 


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
    // step 1 : turn all led off. , send 0x000000 to each led (all 64)
    // step 2 : stop the PRU unit running. i.e make PRUN=0 stop
    TurnOffAllLeds();
}