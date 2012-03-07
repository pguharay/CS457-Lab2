/*
 * common.h
 *
 *  Created on: Mar 7, 2012
 *      Author: jon
 */

#ifndef COMMON_H_
#define COMMON_H_
#define MAX_HOSTS 255


typedef struct serverPortPair{
	char serverAddress[255];
	char port[10];
}ServerPortPairs[MAX_HOSTS];

typedef struct awgetRequest{
	char url[255];
	int numEntries;
	ServerPortPairs hostList;
}AwgetRequest;



#endif /* COMMON_H_ */
