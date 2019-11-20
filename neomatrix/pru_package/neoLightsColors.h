#ifndef NEOLIGHTSCOLORS_H
#define NEOLIGHTSCOLORS_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pru_cfg.h>
#include "resource_table_0.h"
#include "resource_table_empty.h"
#include "prugpio.h"
#include <pru_intc.h>
#include <rsc_types.h>
#include <pru_rpmsg.h>

#endif

// PRU Signal Information ------------------------------------------- 
#define oneCyclesOn     700/5   // Stay on 700ns
#define oneCyclesOff    800/5
#define zeroCyclesOn    350/5
#define zeroCyclesOff   600/5
#define resetCycles     60000/5 // Must be at least 50u, use 60u
#define out 1       // Bit number to output one

// RPMsg_PRU Kernel driver information -----------------------------
#define HOST_INT			((uint32_t) 1 << 30)
#define TO_ARM_HOST			16	
#define FROM_ARM_HOST		17
#define CHAN_NAME			"rpmsg-pru"
#define CHAN_DESC			"Channel 30"
#define CHAN_PORT			30
#define VIRTIO_CONFIG_S_DRIVER_OK	4
char payload[RPMSG_BUF_SIZE];

// GPIO Information ------------------------------------------------

// This should be enabled as pruout or pruin.
#define P9_27 (1<<5) // pruout mode
#define P9_28 (1<<3) // pruout mode
uint32_t gpio = P9_27  ; 

volatile register unsigned int __R30; // output gpio register for prun=0
volatile register unsigned int __R31; // input gpio register for prun=0

uint32_t *gpio1 = (uint32_t *)GPIO1;

// NEOMATRIX 8*8 INFORMATION ---------------------------------------- 

#define NUM_LED_BITS 24 // # bits to represent the color 
#define NUM_LEDS 64 // in Neomatrix 8*8. but this can change for other Adafruit neo versions .
#define MAX_ROWS 8 
#define MAX_COLUMNS 8
// MAX_ROWS * MAX_COLUMNS == NUM_LEDS always

// COLOR INFORMATION ------------------------------------------------ 

#define TOTAL_NUM_COLORS 12

#define COLOR1 0x030200
#define COLOR2 0x020300
#define COLOR3 0x000203
#define COLOR4 0x000302
#define COLOR5 0x020003
#define COLOR6 0x030002
#define COLOR7 0x0f0f00
#define COLOR8 0x0f0f00
#define COLOR9 0x000f0f
#define COLOR10 0x000f0f
#define COLOR11 0x0f0003
#define COLOR12 0x0f000f
 
uint32_t color_palette[TOTAL_NUM_COLORS]={
        COLOR1,
        COLOR2,
        COLOR3,
        COLOR4,
        COLOR5,
        COLOR6,
        COLOR7,
        COLOR8,
        COLOR9,
        COLOR10,
        COLOR11,
        COLOR12,
};

 const int COL1 = {0|8|16|24|32|40|48|56} ; 
 const int COL2 = {1|9|17|25|33|41|49|57} ; 
 const int COL3 = {2|10|18|26|34|42|50|58} ; 
 const int COL4 = {3|11|19|27|35|43|51|59} ; 
 const int COL5 = {4|12|20|28|36|44|52|60} ; 
 const int COL6 = {5|13|21|29|37|45|53|61} ; 
 const int COL7 = {6|14|22|30|38|46|54|62} ; 
 const int COL8 = {7|15|23|31|39|47|55|63} ;
 
 const int ACOL1 = {0&8&16&24&32&40&48&56} ; 
 const int ACOL2 = {1&9&17&25&33&41&49&57} ; 
 const int ACOL3 = {2&10&18&26&34&42&50&58} ; 
 const int ACOL4 = {3&11&19&27&35&43&51&59} ; 
 const int ACOL5 = {4&12&20&28&36&44&52&60} ; 
 const int ACOL6 = {5&13&21&29&37&45&53&61} ; 
 const int ACOL7 = {6&14&22&30&38&46&54&62} ; 
 const int ACOL8 = {7&15&23&31&39&47&55&63} ;



// COLOR COORDINATES ON LED-MATRIX DEFINITION
struct color_matrix  {
    uint32_t color; // 24-bit color
    int x ; // int between 1 and 8
    int y  ;// int between 1 and 8
}  ;


// FUNCTION DEFINITIONS  ------------------------------------------------
// Note : due to some reason, there are perfectly exported.
// for instance, including this header file does not enable these
// functions to be shared with the c file where header is included.
// that's why c file is included instinead of header in neo1.c
void TurnAllBlue(void);
void TurnAllGreen(void);
void TurnAllRed(void);
void TurnLEDColors(void);
void TurnOnAllLeds(void);
void TurnOffAllLeds(void);

