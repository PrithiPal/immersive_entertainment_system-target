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
uint32_t getColorbarColor(int height_counter);
void setLightBarsAll(int height[]);
void test_setlightbar(void); 

//--------------------------------------------------------------------------------
// send turn on message to pru1_1 (__R30 register)
void bit_on(void){
    __R30 |= gpio;      // Set the GPIO pin to 1
    __delay_cycles(oneCyclesOn-1);
    __R30 &= ~(gpio);   // Clear the GPIO pin
    __delay_cycles(oneCyclesOff-2);
}
// send turn off message to pru1_1 (__R30 register)
void bit_off(void){
    __R30 |= gpio;      // Set the GPIO pin to 1
    __delay_cycles(zeroCyclesOn-1);
    __R30 &= ~(gpio);   // Clear the GPIO pin
    __delay_cycles(zeroCyclesOff-2);
}

// This function lits up the neomatrix with light-bars
// repeatedly calling this will creates the effect of 
// real-time audio visualizer.

// this gets called in index=2 in neo1.c
// 1 <= col_num <= 8 and 8 <= height <= 1 
// input is height array which has eight values.
void setLightBarsAll(int height[]){

    uint32_t color[NUM_LEDS] ;
    //uint32_t specific_color ; // later on make arg from function
    int i,j,k ; 
    //int counter = 0 ; 
    //uint8_t r,g,b ; 
    
    int height_counter1 = 8 ; 
    int height_counter2 = 8 ; 
    int height_counter3 = 8 ; 
    int height_counter4 = 8 ; 
    int height_counter5 = 8 ; 
    int height_counter6 = 8 ; 
    int height_counter7 = 8 ; 
    int height_counter8 = 8 ; 

    for(k=0;k<NUM_LEDS;k++){
        //specific_color = getColorbarColor(height_counter);
        //specific_color=0x0000ff;
        
        if(  (k==0) || (k==8) || (k==16) || (k==24) || (k==32) || (k==40) || (k==48) || (k==56)  ) {

            
            if(height_counter1<=height[0]){
                color[k] = getColorbarColor(height_counter1); ; 
            }
            else{
                color[k]=0x000000;
            }
            
            height_counter1--;
        }
        else if(  (k==1) || (k==9) || (k==17) || (k==25) || (k==33) || (k==41) || (k==49) || (k==57)  ){
            
           
            
            if(height_counter2<=height[1]){
                color[k] = getColorbarColor(height_counter2); ; 
            }
            else{
                color[k]=0x000000;
            }
            
            
            
            height_counter2--;
        }
        else if(  (k==2) || (k==10) || (k==18) || (k==26) || (k==34) || (k==42) || (k==50) || (k==58)   ){
            
            
            if(height_counter3<=height[2]){
                color[k] = getColorbarColor(height_counter3); ; 
            }else{
                color[k]=0x000000;
            }
            
            height_counter3--;
        }
        else if(  (k==3) || (k==11) || (k==19) || (k==27) || (k==35) || (k==43) || (k==51) || (k==59)  ){
            
            
            if(height_counter4<=height[3]){
                color[k] = getColorbarColor(height_counter4); ; 
            }else{
                color[k]=0x000000;
            }
            
            height_counter4--;
        }
        else if(  (k==4) || (k==12) || (k==20) || (k==28) || (k==36) || (k==44) || (k==52) || (k==60)   ){
           
            
            if(height_counter5<=height[4]){
                color[k] = getColorbarColor(height_counter5); ; 
            }else{
                color[k]=0x000000;
            }
            
            height_counter5--;
        }
        else if(  (k==5) || (k==13) || (k==21) || (k==29) || (k==37) || (k==45) || (k==53) || (k==61)  ){
            
            
            
            if(height_counter6<=height[5]){
                color[k] = getColorbarColor(height_counter6);
            }else{
                color[k]=0x000000;
            }
            
            height_counter6--;
        }
        else if(  (k==6) || (k==14) || (k==22) || (k==30) || (k==38) || (k==46) || (k==54) || (k==62)   ){
            
            
            
            if(height_counter7<=height[6]){
                color[k] = getColorbarColor(height_counter7);
            }else{
                color[k]=0x000000;
            }
            
            height_counter7--;
        }
        else if(  (k==7) || (k==15) || (k==23) || (k==31) || (k==39) || (k==47) || (k==55) || (k==63)   ){
            
            
            
            if(height_counter8<=height[7]){
                color[k] = getColorbarColor(height_counter8);
            }else{
                color[k]=0x000000;
            }
            
            height_counter8--;
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

// This function lits up the neomatrix with light-bars
// repeatedly calling this will creates the effect of 
// real-time audio visualizer.
uint32_t getColorbarColor(int height_counter){
    
    uint8_t r,g,b;
    //uint32_t specific_color ;
            if(height_counter==8){
                r = 0x00;
                g = 0x00;
                b = 0xb0;  
            }
            else if(height_counter==7){
                r = 0x00;
                g = 0x00;
                b = 0x80;  
            }
            else if(height_counter==6){
                r = 0x00;
                g = 0xb0;
                b = 0x40;  
            }
            else if(height_counter==5){
                r = 0x00;
                g = 0x80;
                b = 0x00;

            }
            else if(height_counter==4){
                r = 0x40;
                g = 0x40;
                b = 0x00;  
            }
            else if(height_counter==3){
                r = 0x80;
                g = 0x30;
                b = 0x00;
            }
            else if(height_counter==2){
                r = 0xb0;
                g = 0x20;
                b = 0x00;  
            }
            else if(height_counter==1){
                r = 0xff;
                g = 0x00;
                b = 0x00;
            }
    
    return g << 16 | r << 8 | b ; 
}


void test_setlightbar(void){
    
    //int i ; 
    
    int height[8] = {1,2,3,4,5,6,7,8};
   
    while(1){
        TurnOffAllLeds();
        __delay_cycles(10000);
        setLightBarsAll(height);
        __delay_cycles(1000000);
        TurnOffAllLeds();    
    }

    //__R30 &= ~(gpio);   
    //__delay_cycles(resetCycles);

}
// for testing purposes.
// move the led from 1st LED to 64th LED.
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

// Creates four quadrants in Neomatrix.
// and lits each quadrant with single color.
// four-color in total on NeoMatrix.
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

// Turns on specific led with r = row, c = column , specific_color = color_hex (GRB format)
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

// Turns all LEDs  blue.
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
// Iterate through colors in NeoMatrix with colors defined in neoLightsColors.h color_palette
void DisplayAllColors(void){

    int i;
    for(i=0;i<TOTAL_NUM_COLORS;i++){
        TurnOffAllLeds();
        __delay_cycles(100000);
        TurnAllCustomColor(color_palette[i]);
        //__delay_cycles(100000000/2);
    }

}
// displays all possible colors by changing color hex every iteration.
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

// turns on all LEDS green on neomatrix
void TurnAllGreen(void){
 
    uint32_t custom_color = 0xff0000; // green color 
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

// turns on all LEDS red on neomatrix
void TurnAllRed(void){
 
    uint32_t custom_color = 0x00ff00; // red color 
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

// testing function.
// gets called in main() in neo1
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

// turn off all leds by sending 
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