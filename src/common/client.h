/*
 * client.h
 *
 *  Created on: Mar 8, 2012
 *      Author: jon
 */

#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include "protocol.h"
#include "util.h"

#ifndef CLIENT_H_
#define CLIENT_H_

/*
typedef struct clientArgument
{
  char*        documentUrl;
  char*        hostFile;
};*/

class AwgetClient {
	public:
		AwgetClient(AwgetRequest request);
		virtual ~AwgetClient();
		const char* awget(); //makes the call to get document and returns the path to the local file.
		const char* sendRequest(SteppingStoneAddress ss, AwgetRequest request);

	private:
		//sockaddr* serverAddress;         /* SERVER HOST ADDRESS */
		//int socketId;
		//bool validateArgument(clientArgument clientArg);
		void initializeConnection(SteppingStoneAddress ss);
		//AwgetRequest    createRequest(char* documentUrl, SteppingStoneAddress steppingStones[]); //creates awget request from stepping stones and doc url.
		//SteppingStoneAddress* getSteppingStonesFromFile(char* fi); //creates an array of stepping stones from chaingang file.
		SteppingStoneAddress getRandomSteppingStoneAddressFromList(SteppingStoneAddress steppingStones[], uint8_t size); //gets a random stepping stone address from list.
		/*makes the blocking TCP call to a random stepping stone and waits for response.
		 *returns  the raw byte array of the document.*/
		//const char* sendRequest(SteppingStoneAddress* ss, AwgetRequest* request);
};





#endif /* CLIENT_H_ */
