#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#include <stdbool.h>
#include <ctype.h>
#include <time.h>

#define MAX_RGB_BUFFER_SIZE 1024
char output_buff[MAX_RGB_BUFFER_SIZE] = ""; // shared resource between read_rgb_file thread and write_prmsg_driver thread.

// ENV VARIABLE DEFINITIONS 
const char *rgb_output_filename;
const char *prmsg_device_filename ;


// FUNCTION DECALRATIONS -----------------------------------------------------------------
void substring(char big_string[], char substr[] , int position , int length ); 
void convert_rgb_to_grb(char rgb[], char grb[]);
void write_output_buff(char new_buff[]) ; 
void read_output_buff(char local_buff[]);
// FUNCTION DEFINITIONS  -----------------------------------------------------------------

// Credits : https://www.programmingsimplified.com/c/source-code/c-substring
void substring(char s[], char sub[], int p, int l) {
    int counter = 0 ; 
    while(counter < l){
        sub[counter]=s[p+counter-1];
        counter++;
    }
    sub[counter]='\0';
}

void convert_rgb_to_grb(char rgb[] , char grb[] ){
        char red[5], blue[5], green[5];
        
        substring(rgb,red,1,2);
        substring(rgb,green,3,2);
        substring(rgb,blue,5,2);
        //printf("red = %s, green = %s, blue = %s\n",red,green,blue);
        
       
        strcat(grb,green);
        strcat(grb,red);
        strcat(grb,blue);
        strcat(grb,"\0");
        
        //printf("new grb = %s\n",grb);
    
}

// thread function to continuously read values from the RGB_SCREENCAPTURE_FILE values.
void* read_rgb_file(void* dv) {
    printf("[READ_THREAD] : read_rgb_file Thread starting ... \n");
    // endlessly check for the ~/rgb_values.txt.
    while(1){
        FILE *rgb_file ;
        rgb_file = fopen(rgb_output_filename,"r");
        if(!rgb_file){
            printf("[READ_THREAD] : Cannot open file with path= %s\n",rgb_output_filename);
        }
        
     
        int mode_num ; 
        fscanf(rgb_file,"%d\n",&mode_num); // update the buffer.

        if(mode_num==0){
            
            char top_left[10], top_right[10], bottom_left[10], bottom_right[10] ; 
            char grb_top_left[10] = "";
            char grb_top_right[10] = "";
            char grb_bottom_left[10] = "";
            char grb_bottom_right[10] = "";  

            //printf("[READ_THREAD] : mode number 0 selected \n ");
            fscanf(rgb_file,"%s",top_left);
            fscanf(rgb_file,"%s",top_right);
            fscanf(rgb_file,"%s",bottom_left);
            fscanf(rgb_file,"%s",bottom_right);
            //printf("[READ_THREAD] : RGB a = %s, b = %s, c = %s, d = %s\n",top_left, top_right, bottom_left, bottom_right);
            
            convert_rgb_to_grb(top_left,grb_top_left);
            convert_rgb_to_grb(top_right,grb_top_right);
            convert_rgb_to_grb(bottom_left,grb_bottom_left);
            convert_rgb_to_grb(bottom_right,grb_bottom_right);

            char buff[1024];
            sprintf(buff,"0 0x%s 0x%s 0x%s 0x%s",grb_top_left,grb_top_right,grb_bottom_left,grb_bottom_right);
           
            write_output_buff(buff);
            //printf("[READ_THREAD] : GRB a = %s, b = %s, c = %s, d = %s\n",grb_top_left, grb_top_right, grb_bottom_left, grb_bottom_right);

        }
        // single dominant color hex value 
        else if(mode_num==1){
            //printf("mode number 1 selected \n ");
            char rgb_dominant_color[10],grb_dominant_color[10]=""; 
            
            fscanf(rgb_file,"%s",rgb_dominant_color);
            //printf("[READ_THREAD] : %s contents = %s\n",rgb_output_filename,rgb_dominant_color);
            convert_rgb_to_grb(rgb_dominant_color,grb_dominant_color);
            
            
            char buff[1024];
            sprintf(buff,"1 0x%s",grb_dominant_color);

            //printf("[READ_THREAD] : Color in GRB = %s\n",grb_dominant_color);
            //printf("to write =  %s\n",buff);
            write_output_buff(buff);
        }
        // Audio Visulizer output from audioAverager.c
        else if(mode_num == 2){
            char buff[1024];

            char val1[100] = "";
            char val2[100] = "";
            char val3[100] = "";
            char val4[100] = "";
            char val5[100] = "";
            char val6[100] = "";
            char val7[100] = "";
            char val8[100] = "";


            fscanf(rgb_file,"%s",val1);
            fscanf(rgb_file,"%s",val2);
            fscanf(rgb_file,"%s",val3);
            fscanf(rgb_file,"%s",val4);
            fscanf(rgb_file,"%s",val5);
            fscanf(rgb_file,"%s",val6);
            fscanf(rgb_file,"%s",val7);
            fscanf(rgb_file,"%s",val8);


            sprintf(buff,
                    "2 %s %s %s %s %s %s %s %s",
                    val1,
                    val2,
                    val3,
                    val4,
                    val5,
                    val6,
                    val7,
                    val8
                    );


            write_output_buff(buff);
        }
        else if(mode_num == 3){
            //printf("mode number 1 selected \n ");
            char rgb_dominant_color[10],grb_dominant_color[10]=""; 
            
            fscanf(rgb_file,"%s",rgb_dominant_color);
            //printf("[READ_THREAD] : %s contents = %s\n",rgb_output_filename,rgb_dominant_color);
            convert_rgb_to_grb(rgb_dominant_color,grb_dominant_color);
            
            
            char buff[1024];
            sprintf(buff,"1 %s",grb_dominant_color);

            //printf("[READ_THREAD] : Color in GRB = %s\n",grb_dominant_color);
            //printf("to write =  %s\n",buff);
            write_output_buff(buff);

        }

        fclose(rgb_file);
    }


    return NULL ; 

}

// used by read_rgb_file thread
void write_output_buff(char new_buff[]){
    
    static pthread_mutex_t my_lock = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&my_lock);
    
    //printf("to write =  %s\n",new_buff);
    strcpy(output_buff,new_buff);
    
    pthread_mutex_unlock(&my_lock);

}

void* write_prmsg_driver(void* dv){
    printf("[WRITE_THREAD] : write_prmsg_driver function starting ... \n");
    
    char local_buff[MAX_RGB_BUFFER_SIZE]=""; 
    char old_buff[MAX_RGB_BUFFER_SIZE]="0" ; 
    
    while(1){

        read_output_buff(local_buff);
        //printf("local_buff = %s\n",local_buff);
        //printf("[WRITE_THREAD] : Updating..\n");
    
        // It will write to /dev/prmsg_pru30 if and only if there has been change since the last value.
        if(strncmp(local_buff,old_buff,MAX_RGB_BUFFER_SIZE)!=0){

            printf("[WRITE_THREAD] : Buff value changed !!\n");
            printf("old_buff = %s, new_buff = %s\n",old_buff,local_buff);
            

            char permissions_cmd[MAX_RGB_BUFFER_SIZE] ; 
            sprintf(permissions_cmd,"sudo chmod 666 %s",prmsg_device_filename);
            printf("[WRITE_THREAD] : permission_cmd = %s\n",permissions_cmd);
            system(permissions_cmd);

            char write_cmd[MAX_RGB_BUFFER_SIZE];
            sprintf(write_cmd,"echo %s | sudo tee %s",local_buff,prmsg_device_filename);
            
        
            printf("[WRITE_THREAD] : write_cmd = %s\n",write_cmd);
            system(write_cmd);
        
        }
        sprintf(old_buff,"%s",local_buff);
        
    }

    return NULL;
}

// used by write_prmsg_driver thread
void read_output_buff(char local_buff[]){

    //printf("read_output_buff starting ... \n");
    static pthread_mutex_t my_lock = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&my_lock);
    
    strcpy(local_buff,output_buff);
    //printf("local_buff in read_output_buff = %s\n",output_buff);
    pthread_mutex_unlock(&my_lock);

}




int main(void){
    printf("Hello world from neomatrix_interface.c \n");
    
    rgb_output_filename=    getenv("RGB_SCREENCAPTURE_FILE");	
    prmsg_device_filename = getenv("PRMSG_FILENAME");	

    
    pthread_t tid1,tid2; 
    pthread_create(&tid1,NULL,&read_rgb_file,NULL);

    sleep(1); // wait for the output_buff to be filled before consumed by write_prmsg_driver()

    pthread_create(&tid2,NULL,&write_prmsg_driver,NULL);

     // wait for the output_buff to be filled before consumed by write_prmsg_driver()
    
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    
    return 0 ; 
}