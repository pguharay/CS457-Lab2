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

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

typedef struct __attribute__((packed)) SteppingStoneAddress
{
	char hostAddress[255];
	uint8_t port;
}ssAddress;


typedef struct __attribute__((packed)) AwgetRequest
{
	char 		url[255];
	uint8_t	chainListSize;
	ssAddress   chainList[255];
}awgetRequest;


#endif /* STEPPING_STONE_H_ */
