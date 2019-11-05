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

	char * currentStringIndex;
	currentStringIndex = malloc(sizeof(*currentStringIndex) * 1);

	char * lastStringIndex;
	lastStringIndex = malloc(sizeof(*lastStringIndex) * 1);
	int ret = 0;
	do{

		ssize_t recvResponse = recvfrom(socketDescriptor, message, UDP_MESSAGE_SIZE, 0, (struct sockaddr *) &addr, &addrlen);

		printf("[client-target] recvResponse: %d\n", recvResponse);
		//printf("[client-target] message recieved : \n{%s}\n", message);

		char * timestamp;
		timestamp = malloc(sizeof(*timestamp) * EPOCH_TIMESTAMP_LENGTH);

		int beginningIndexOfFirstTimestamp = 0, endingIndexOfFirstTimestamp = 0, beginningIndexOfLastTimestamp = 0, endingIndexOfLastTimestamp = 0;

		beginningIndexOfFirstTimestamp = EPOCH_TIMESTAMP_LENGTH+1;

		for (int i = EPOCH_TIMESTAMP_LENGTH; endingIndexOfLastTimestamp == 0; i++){
			if (message[i] == '/'){
				endingIndexOfFirstTimestamp = i;
				beginningIndexOfLastTimestamp = i + 1;
			}
			if (message[i] == ']'){
				endingIndexOfLastTimestamp = i;
			}
		}


		char * currentStringIndex = printSubString(message, beginningIndexOfFirstTimestamp,  endingIndexOfFirstTimestamp);
		char * lastStringIndex = printSubString(message, beginningIndexOfLastTimestamp,  endingIndexOfLastTimestamp);

		printf("timestamp : %s\n", printSubString(message, 0,  30) );
		printf("[client-target] beginningIndexOfFirstTimestamp: %d\n", beginningIndexOfFirstTimestamp);
		printf("[client-target] endingIndexOfFirstTimestamp : {%d}\n", endingIndexOfFirstTimestamp);
		printf("[client-target] beginningIndexOfLastTimestamp: %d\n", beginningIndexOfLastTimestamp);
		printf("[client-target] endingIndexOfLastTimestamp : {%d}\n", endingIndexOfLastTimestamp);
		printf("timestamp : %s\n", printSubString(message, EPOCH_TIMESTAMP_INDEX,  EPOCH_TIMESTAMP_LENGTH) );
		printf("currentStringSize : %s\n", currentStringIndex );
		printf("totalStringSize : %s\n", lastStringIndex );

		ret = strcmp(currentStringIndex, lastStringIndex);

		printf("{%d}\n", ret);

		//{[1572944379-0065471/6040921]


	}while(ret != 0);


	return 0;

}

char * printSubString(char * message, long firstIndex, long endIndex){
	char * substr;
	substr = malloc(sizeof(*substr) * (endIndex - firstIndex+1));

	//std::cout<<"endIndex - firstIndex : " << endIndex - firstIndex << std::endl;
	int index = 0;
	for (long i = firstIndex; index < endIndex - firstIndex; i++){
		//std::cout<<" index : " << index << std::endl;
		substr[index++] = message[i];
	}
	substr[endIndex - firstIndex]=0;
	return substr;

}
