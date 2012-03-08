#include "../common/stepping_stone.h"
#include "../common/util.h"

fd_set masterSet;
fd_set readSet;
int maxFd = 0;

void* startService(void* argument)
{
	StartArgument* startArg = (StartArgument*) argument;

	int status = listen(startArg->listenerSocket, BACKLOG);

	if (status == FAILURE) {
		perror("Unable to listen on bind address");
		pthread_exit(NULL);
	}

	selectConnection(startArg->listenerSocket);

	pthread_exit(NULL);

	return NULL;
}

void selectConnection(int listenerSocket)
{
  initFileDescriptorSet(listenerSocket);

  struct timeval timeout;
  timeout.tv_sec = 2;
  timeout.tv_usec = 0;

  while(true)
  {
    readSet = masterSet;

    int status= select(maxFd + 1, &readSet, NULL, NULL, &timeout);

    if(status == FAILURE)
    {
      perror("Unable to select");
      pthread_exit(NULL);
    }

    probeConnection(maxFd, listenerSocket);
  }
}

void probeConnection(int maxFd, int listenerSocket)
{
   int i = 0;
   for(i=0;i<=maxFd;i++)
   {
      if(FD_ISSET(i, &readSet))
      {
    	 if(i == listenerSocket)
    	 {
    		 acceptConnection(listenerSocket);
    	 }
    	 else
    	 {
    		 receiveData(i);
    	 }
      }
   }
}

void acceptConnection(int socketid)
{
  sockaddr_in clientAddress;
  socklen_t   socketLength = sizeof(clientAddress);
  int connectionSocket = accept(socketid, (struct sockaddr*)&clientAddress, &socketLength);

  if(connectionSocket < 0)
  {
    perror("Error occurred while accepting connection from client");
  }
  else
  {
    FD_SET(connectionSocket, &masterSet);
    if(connectionSocket > maxFd)
    {
      maxFd = connectionSocket;
    }
  }
}

void receiveData(int socketid)
{
	AwgetRequest request;
	int status = receiveOnTCPSocket(socketid, &request, sizeof(request));

	if(status == SUCCESS)
	{
		debug("Request = URL %s \n", request.url);
		info("chain list =[ \n");
		for(int i = 0;i<request.chainListSize; i++)
		{
			SteppingStoneAddress ssAddress = request.chainList[i];
			debug("<%s>,<%u> \n", ssAddress.hostAddress, ssAddress.port);
		}
		info("]");
	}

    close(socketid);
    FD_CLR(socketid, &masterSet);
}

int receiveOnTCPSocket(int socketid, AwgetRequest* request, size_t length)
{
  int bytes = recv(socketid, (void*)request, length, 0);

  debug("Received %d bytes \n", bytes);

  return SUCCESS;
}

void initFileDescriptorSet(int socketid)
{
  maxFd = socketid;
  FD_ZERO(&masterSet);
  FD_ZERO(&readSet);
  FD_SET(socketid, &masterSet);
}
