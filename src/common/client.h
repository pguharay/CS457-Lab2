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
#include <fstream>
#include <fcntl.h>
#include <vector>
#include <algorithm>
#include "awget.h"
#include "util.h"
using namespace std;

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
		const char* requestAndSaveFile(SteppingStoneAddress ss, AwgetRequest request);
		SteppingStoneAddress dequeRandomSteppingStoneAddressFromList(AwgetRequest* awgetRequest, uint16_t size);

	private:
		void initializeConnection(SteppingStoneAddress ss);
		SteppingStoneAddress getRandomSteppingStoneAddressFromList(SteppingStoneAddress steppingStones[], uint8_t size); //gets a random stepping stone address from list.

};





#endif /* CLIENT_H_ */
