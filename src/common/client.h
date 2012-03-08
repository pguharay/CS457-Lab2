/*
 * client.h
 *
 *  Created on: Mar 8, 2012
 *      Author: jon
 */
#include "protocol.h"


#ifndef CLIENT_H_
#define CLIENT_H_




class client {
public:
	client(char* documentUrl, char* ssfileLocation);
	client(char* documentUrl);
	virtual ~client();
	void sendRequest(AwgetRequest request);
	char* receieveResponse();

private:
	char* defaultFileLocation;
	bool validateArgument(clientArgument* clientArg);
	AwgetRequest createRequest(char* documentUrl, SteppingStone* steppingStones);
	SteppingStoneAddress* getSteppingStonesFromFile(char* fi);
	SteppingStoneAddress getRandomSteppingStoneAddressFromList(SteppingStone* steppingStones);
};


typedef struct clientArgument
{
  char*        documentUrl;
  char*        hostFile;
}clientArg;


#endif /* CLIENT_H_ */
