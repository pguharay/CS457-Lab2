#include "../common/stepping_stone.h"
#include "../common/util.h"

fd_set masterSet;
fd_set readSet;
int maxFd = 0;
pthread_t handler[MAX_THREAD];
pthread_mutex_t threadMutex;
int threadIndex = 0;
FileRetrieverService* fileRetriever;

void* startService(void* argument)
{
	StartArgument* startArg = (StartArgument*) argument;

	srand(time(NULL));

	int status = listen(startArg->listenerSocket, BACKLOG);

	if (status == FAILURE)
	{
		perror("Unable to listen on bind address");
		pthread_exit(NULL);
	}

	fileRetriever = new FileRetrieverService();

	pthread_mutex_init(&threadMutex, NULL);

	selectConnection(startArg->listenerSocket);

	pthread_mutex_destroy(&threadMutex);

	pthread_exit(NULL);

	delete(fileRetriever);

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

    pthread_mutex_lock(&threadMutex);

    int status= select(maxFd + 1, &readSet, NULL, NULL, &timeout);

    pthread_mutex_unlock(&threadMutex);

    if(status == FAILURE)
    {
      perror("Unable to select");
      pthread_exit(NULL);
    }
    else
    {
    	probeConnection(maxFd, listenerSocket);
    }
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

void acceptConnection(int listenerSocket)
{
  sockaddr_in clientAddress;
  socklen_t   socketLength = sizeof(clientAddress);

  int connectionSocket = accept(listenerSocket, (struct sockaddr*)&clientAddress, &socketLength);

  if(connectionSocket < 0)
  {
    perror("Error occurred while accepting connection from client");
    pthread_exit(NULL);
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

	if (status == SUCCESS)
	{
		debug("Request = URL %s \n", request.url);
		debug("ChainListl size = %u \n", ntohs(request.chainListSize));
		info("SS list =[ \n");
		for (int i = 0; i < ntohs(request.chainListSize); i++)
		{
			SteppingStoneAddress ssAddress = request.chainList[i];
			debug("<%s>,<%u> \n", ssAddress.hostAddress, ntohl(ssAddress.port));
		}

		info("] \n");

		handleRequestAsync(socketid, request);
	}
	else
	{
		pthread_mutex_lock(&threadMutex);

		FD_CLR(socketid, &masterSet);
		close(socketid);

		pthread_mutex_unlock(&threadMutex);
	}
}

void handleRequestAsync(int socketid, AwgetRequest awgetRequest)
{
	pthread_attr_t handlerAttribute;

	TaskParameter* taskParameter = (TaskParameter*) malloc(sizeof(TaskParameter));

	if(taskParameter == NULL)
	{
		perror("OutOfMemoryError - unable to allocate memory for new object");
		exit(1);
	}

	taskParameter->socketid = socketid;
	taskParameter->awgetRequest = awgetRequest;

	pthread_mutex_lock(&threadMutex);

	int index = 0;
	if (threadIndex < MAX_THREAD) {
		index = threadIndex++;
	} else {
		index = threadIndex = 0;
	}

	pthread_mutex_unlock(&threadMutex);

	pthread_attr_init(&handlerAttribute);
	pthread_attr_setdetachstate(&handlerAttribute, PTHREAD_CREATE_JOINABLE);
	pthread_create(&handler[index], &handlerAttribute, &handleRequest, (void*) taskParameter);
}

void* handleRequest(void* argument)
{
	TaskParameter* taskParameter = (TaskParameter*)argument;

	try
	{
		if (ntohs(taskParameter->awgetRequest.chainListSize) > 0)
		{
			info("waiting ... \n");

			fileRetriever->handleRequest(taskParameter->awgetRequest,taskParameter->socketid);
		}
		else
		{
			debug("This is the last SS, wget %s \n", taskParameter->awgetRequest.url);

			fileRetriever->wget(taskParameter->awgetRequest.url, taskParameter->socketid);
		}
	}
	catch (char* errorMessage)
	{
		error(errorMessage);
	}

	info("Good bye . \n");

	pthread_mutex_lock(&threadMutex);

	FD_CLR(taskParameter->socketid, &masterSet);
	close(taskParameter->socketid);
	free(taskParameter);

	pthread_mutex_unlock(&threadMutex);

	return NULL;
}

int receiveOnTCPSocket(int socketid, AwgetRequest* request, size_t length)
{
	info("Receiving request... \n");

	int byte = recv(socketid, (void*)request, length, 0);

	if(byte != sizeof(AwgetRequest))
	{
		error("Unable to receive the request \n.");
		return FAILURE;
	}
	else
	{
		return SUCCESS;
	}
}

void initFileDescriptorSet(int socketid)
{
  maxFd = socketid;
  FD_ZERO(&masterSet);
  FD_ZERO(&readSet);
  FD_SET(socketid, &masterSet);
}
