#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>



#include <stdbool.h>
#include <ctype.h>

#define MAX_RGB_BUFFER_SIZE 1024
char buff[MAX_RGB_BUFFER_SIZE]; // shared resource between read_rgb_file thread and write_prmsg_driver thread.

// ENV VARIABLE DEFINITIONS 

const char *rgb_output_filename;
//const char *prmsg_device_filename;

// thread function to continuously read values from the RGB_SCREENCAPTURE_FILE values.
void read_rgb_file(void){

    FILE *rgb_file ;
    rgb_file = fopen(rgb_output_filename,"r");
    if(!rgb_file){
        printf("Cannot open file with path= %s\n",rgb_output_filename);
    }
    
    printf("File opened\n");
    
    int mode_num ; 
    fscanf(rgb_file,"%d\n",&mode_num); // update the buffer.
    printf("buff=%s\n",buff);
    
    // four region hex colors given.
    if(mode_num==0){
        
        int top_left, top_right, bottom_left, bottom_right ; 
        printf("mode number 0 selected \n ");
        fscanf(rgb_file,"%d",&top_left);
        fscanf(rgb_file,"%d",&top_right);
        fscanf(rgb_file,"%d",&bottom_left);
        fscanf(rgb_file,"%d",&bottom_right);
        printf("a = %d, b = %d, c = %d, d = %d\n",top_left, top_right, bottom_left, bottom_right);
    
    }
    else if(mode_num==1){
        printf("mode number 1 selected \n ");
        int dominant_color ; 
        fscanf(rgb_file,"%d",&dominant_color);
        printf("color = %d\n",dominant_color);
    }
    
    
 
}

/*
void* write_prmsg_driver(void *message){
    FILE *prmsg_device_file ;
    prmsg_device_file = fopen(PRMESG_DEVICE_FILE,"w");
    fprintf(prmsg_device_filename,(char *)message);
}
*/

int main(void){
    printf("Hello world from neomatrix_interface.c \n");
    
    rgb_output_filename=    getenv("RGB_SCREENCAPTURE_FILE");	
    //const char *prmsg_device_filename = getenv("PRMESG_DEVICE_FILE");	

    read_rgb_file();

    return 0 ; 
}