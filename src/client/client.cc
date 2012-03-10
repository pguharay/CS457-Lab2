/*
 * client.cpp
 *
 *  Created on: Mar 8, 2012
 *      Author: jon
 */

#include "../common/client.h"
#include "../common/protocol.h"


clientArgument args;
struct addrinfo  serverAddressQuery;
struct addrinfo* serverAddressResultList;


client::client(clientArgument* clientArg) {
	args = clientArg;
}

char* sendRequest(AwgetRequest* request){
	//get a random steppingstone from the list in the request.
	SteppingStoneAddress ss = getRandomSteppingStoneAddressFromList(request->chainList, request->chainListSize);
	//open a tcp connection and send AwgetRequest to steppingstone
	//resolve the hostAddress from the steppingstone structure...
	memset(&serverAddressQuery, 0x0000, sizeof(serverAddressQuery));
	serverAddressQuery.ai_family = AF_INET;
	serverAddressQuery.ai_socktype = SOCK_STREAM;

	int status = getaddrinfo(ss.hostAddress,ss.port, &serverAddressQuery, &serverAddressResultList);

	if(status != SUCCESS)
	{
		error("Unable to get server host information \n");
		exit(1);
	}

	serverAddress = serverAddressResultList->ai_addr;

	socketid = socket(serverAddressResultList->ai_family, serverAddressResultList->ai_socktype, 0);

	 if(socket < 0)
	 {
	    perror("Unable to create socket \n");
	    exit(1);
	 }

	//wait for response, will need a timeout here...

}



SteppingStoneAddress getRandomSteppingStoneAddressFromList(SteppingStoneAddress steppingStones[], uint8_t size){
	/* initialize random seed: */
	srand ( time(NULL) );
	int randomIndex = rand() % size;
	return steppingStones[randomIndex];
}

client::~client() {

}
