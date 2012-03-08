#include "protocol.h"

#ifndef STEPPING_STONE_H_
#define STEPPING_STONE_H_

#define BACKLOG 10

typedef struct StartArgument
{
  int listenerSocket;
  struct sockaddr_in* address;
}StartArg;

class SteppingStone
{
	private:
		addrinfo  addressInfo;
		addrinfo* addrInfoResultList;
		pthread_t starterThread;
		pthread_attr_t starterThreadAttribute;
		StartArgument startArgument;
		char hostname[255];

		void initializeAddressInfo();
		void handleRequest(awgetRequest* request);
		void wget(char* url);
		uint8_t getNextStone(int chainListLength);
		char* getHostName();
		int bindToAddress(addrinfo* iterator, char* port);
		void setupStartParameter(addrinfo* addrinfo, int listenerSocket);

	public:
		SteppingStone(char* port);
		virtual ~SteppingStone();
		virtual void start();
};

void* startService(void*);
void selectConnection(int listenerSocket);
void probeConnection(int maxFd, int listenerSocket);
void acceptConnection(int socketid);
void receiveData(int socketid);
int receiveOnTCPSocket(int socketid, AwgetRequest* request, size_t length);
void initFileDescriptorSet(int socketid);

#endif
