#include "multicast.h"


// being defined because of the following issue
// https://stackoverflow.com/questions/5582211/what-does-define-gnu-source-imply
// https://stackoverflow.com/questions/16927613/error-when-compiling-a-multicast-listener/42236615#42236615
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdlib.h>				// for srand
#include <stdbool.h>
#include <string.h>
#include <ctype.h>


#define EPOCH_TIMESTAMP_LENGTH 11
#define EPOCH_TIMESTAMP_INDEX 1

char * printSubString(char * message, long firstIndex, long endIndex);


int main(){
	int socketDescriptor = socket(AF_INET, SOCK_DGRAM, 0);

	printf("[client-target] socketDescriptor: %d\n", socketDescriptor);

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(MULTICAST_PORT);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	int bindResponse = bind(socketDescriptor, (struct sockaddr *) &addr, sizeof(addr));

	printf("[client-target] bindResponse: %d\n", bindResponse);
	struct ip_mreq mreq;

	const char* s = getenv("INTERFACE_ADDR");

	if ( s == NULL){
		printf("the environment variable \"INTERFACE_ADDR\" has not been exported. Please read through README again\n.");
		return -1;
	}

	mreq.imr_multiaddr.s_addr = inet_addr(MULTICAST_ADDR);
	mreq.imr_interface.s_addr = inet_addr(s);
	
	
	int setsockoptResponse = setsockopt(socketDescriptor, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));

	printf("[client-target] setsockoptResponse: %d\n", setsockoptResponse);

	char * message;
	message = malloc(sizeof(*message) * UDP_MESSAGE_SIZE);
	for (int i = 0; i < UDP_MESSAGE_SIZE; i++){
		message[i] = ' ';
	}
	message[UDP_MESSAGE_SIZE-1] = 0;

	unsigned addrlen = sizeof (addr);
	
	// File to write to so that neomatrix_interface can look and write to prmsg_pru30 driver file
	const char* rgb_output_filename= getenv("RGB_SCREENCAPTURE_FILE");
	
	/*
	FILE *rgb_output_file = fopen(rgb_output_filename,"w");
	if(rgb_output_file==NULL){
		printf("[client-target] File cannot be opened \n");
	}
	printf("RGB_SCREENCAPTURE_FILE  = %s \n",rgb_output_filename); 
	*/

	char write_cmd[2048]  ;
	char messageToWrite[1024] ;

	while(1){


		ssize_t recvResponse = recvfrom(socketDescriptor, message, UDP_MESSAGE_SIZE, 0, (struct sockaddr *) &addr, &addrlen);

		printf("[client-target] recvResponse: %d\n", recvResponse);
		printf("[client-target] message recieved : \n{%s}\n", message);


		int firstComma = 0, secondComma = 0, thirdComma = 0, fourthComma = 0;
		for (int i = 1; message[i] != 0; i++){
			if (message[i] == ','){
				if (firstComma == 0){
					firstComma = i;
				}else if (secondComma == 0){
					secondComma = i;
				}else if (thirdComma == 0){
					thirdComma = i;
				}else if (fourthComma == 0){
					fourthComma = i;
				}
			}
		}


		if (firstComma == 0){

			printf("dominantColor : {%s} \n", message);
			
			//fprintf(rgb_output_file,"1 %s",message);
			
			sprintf(messageToWrite,"1 %s",message);
			sprintf(write_cmd,"echo %s | tee %s ",messageToWrite,rgb_output_filename);
			system(write_cmd);
			
		}
		else{
			//printf("firstComma : {%d}, secondComma : {%d}, thirdComma : {%d}, fourthComma : {%d} \n", firstComma, secondComma, thirdComma, fourthComma);
			char * topLeft, * topRight, * bottomLeft, * bottomRight;
			topLeft = printSubString(message, 0, firstComma);
			topRight = printSubString(message, firstComma+2, secondComma);
			bottomLeft = printSubString(message, secondComma+2, thirdComma);
			bottomRight = printSubString(message, thirdComma+2, fourthComma);

			printf("topLeft : {%s}, topRight : {%s}, bottomLeft : {%s}, bottomRight : {%s} \n", topLeft, topRight, bottomLeft, bottomRight);
			
			//fprintf(rgb_output_file,"0 %s %s %s %s",topLeft,topRight,bottomLeft,bottomRight);
			
			sprintf(messageToWrite,"0 %s %s %s %s",topLeft,topRight,bottomLeft,bottomRight);
			sprintf(write_cmd,"echo %s | tee %s ",messageToWrite,rgb_output_filename);
			system(write_cmd);
			
		}

	}
	//fclose(rgb_output_file);
	return 0;

}

char * printSubString(char * message, long firstIndex, long endIndex){
	char * substr;
	substr = malloc(sizeof(*substr) * (endIndex - firstIndex+1));

	int index = 0;
	for (long i = firstIndex; index < endIndex - firstIndex; i++){
		substr[index++] = message[i];
	}
	substr[endIndex - firstIndex]=0;
	return substr;

}
