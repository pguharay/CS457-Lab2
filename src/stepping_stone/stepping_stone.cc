#include "../common/stepping_stone.h"
#include "../common/util.h"

SteppingStone :: SteppingStone(const char* port)
{
	if(port == NULL)
	{
		port = std::string(DEFAULT_PORT).c_str();
	}

	debug("Starting Stepping stone on port %s \n", port);

	initializeAddressInfo();

	int status = getaddrinfo(getHostName(), port, &addressInfo, &addrInfoResultList);

	if(status != 0)
	{
	    perror("unable to retrieve server address information");
	    exit(1);
	}

	setupStartParameter(addrInfoResultList, bindToAddress(addrInfoResultList, port));
}

SteppingStone :: ~SteppingStone()
{
	info("Server is stopping.\n");
}

void SteppingStone :: start()
{
	  void* status = NULL;
	  pthread_attr_init(&starterThreadAttribute);
	  pthread_attr_setdetachstate(&starterThreadAttribute,PTHREAD_CREATE_JOINABLE);
	  pthread_create(&starterThread, &starterThreadAttribute, &startService, (void*)&startArgument);
	  pthread_join(starterThread, &status);
	  freeaddrinfo(addrInfoResultList);
	  pthread_exit(NULL);
}

void SteppingStone :: initializeAddressInfo()
{
  memset(&addressInfo, 0x0000, sizeof(addressInfo));
  addressInfo.ai_family = AF_INET;
  addressInfo.ai_socktype = SOCK_STREAM;
  addressInfo.ai_flags = AI_PASSIVE;
}

char* SteppingStone::getHostName()
{
    size_t hostnameLength = 255;

    gethostname(hostname, hostnameLength);
    char* host = hostname;

    return host;
}

void SteppingStone :: setupStartParameter(addrinfo* addrinfo, int listenerSocket)
{
	startArgument.listenerSocket = listenerSocket;
	startArgument.address = (sockaddr_in*)addrinfo->ai_addr;
}

int SteppingStone :: bindToAddress(addrinfo* iterator, const char* port)
{
  int listenerSocket;
  int reuse = 1;
  uint32_t ssPort = atoi(port);

  while(iterator != NULL)
  {
    listenerSocket = socket(AF_INET, SOCK_STREAM, 0);

    if(listenerSocket < 0)
    {
      perror("Unable to create socket");
      exit(1);
    }

    ((sockaddr_in*)iterator->ai_addr)->sin_port = htons(ssPort);

    int status = setsockopt(listenerSocket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));

    if(status == FAILURE)
    {
      perror("can not reuse address");
      exit(1);
    }

    status = bind(listenerSocket, iterator->ai_addr, iterator->ai_addrlen);

    if(status == FAILURE)
    {
      perror("Unable to bind to address");
      iterator = iterator->ai_next;
      continue;
    }

    break;
  }

  if(iterator == NULL)
  {
    error("Unable to bind. \n");
    exit(1);
  }

  return listenerSocket;
}
