#include "protocol.h"

#ifndef STEPPING_STONE_H_
#define STEPPING_STONE_H_

#define BACKLOG 10

using namespace std;

typedef struct StartArgument
{
  int listenerSocket;
  struct sockaddr_in* address;
}StartArg;

typedef struct TaskParameter
{
	int socketid;
	AwgetRequest* awgetRequest;
}TaskParam;

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
		uint8_t getNextStone(int chainListLength);
		char* getHostName();
		int bindToAddress(addrinfo* iterator, char* port);
		void setupStartParameter(addrinfo* addrinfo, int listenerSocket);

	public:
		SteppingStone(char* port);
		virtual ~SteppingStone();
		virtual void start();
};

class FileRetrieverService
{
	private:
		string LOCAL_FILE_DIR;
		string getFileLocation(string filename);
		string createWgetCommand(string fileLocation, string urlAsString);
		void readFileAndStream(string fileLocation, int socketid);
    void removeTemporaryFile(string fileLocation);

	public:
		void handleRequest(AwgetRequest* request, int socketid);
		void wget(char* url, int socketid);
};

void* startService(void*);
void selectConnection(int listenerSocket);
void probeConnection(int maxFd, int listenerSocket);
void acceptConnection(int socketid);
void receiveData(int socketid);
int receiveOnTCPSocket(int socketid, AwgetRequest* request, size_t length);
void initFileDescriptorSet(int socketid);
void* invokeFileRetriever(void* argument);
#endif
