#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <pthread.h>
#include <string>
#include <fstream>

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#define MAX_FILE_SIZE 65535

typedef struct __attribute__((packed)) SteppingStoneAddress
{
	char hostAddress[20];
	uint32_t port;
}ssAddress;


typedef struct __attribute__((packed)) AwgetRequest
{
	uint16_t	chainListSize;
	char 		url[255];
	ssAddress   chainList[5];
}awgetRequest;

#endif











=======
#endif /* STEPPING_STONE_H_ */
>>>>>>> branch 'master' of ssh://git@github.com:22/pguharay/CS457-Lab2.git
