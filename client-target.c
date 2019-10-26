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
	message = malloc(sizeof(*message) * PACKET_SIZE);
	for (int i = 0; i < PACKET_SIZE; i++){
		message[i] = ' ';
	}
	message[PACKET_SIZE-1] = 0;

	unsigned addrlen = sizeof (addr);
	ssize_t recvResponse = recvfrom(socketDescriptor, message, PACKET_SIZE, 0, (struct sockaddr *) &addr, &addrlen);

	printf("[client-target] recvResponse: %d\n", recvResponse);
	printf("[client-target] message recieved : \n{%s}\n", message);
	return 0;

}
