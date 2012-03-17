/*
 * client.cpp
 *
 *  Created on: Mar 8, 2012
 *      Author: jon
 */
#include "../common/client.h"
#include "../common/util.h"

struct addrinfo  serverAddressQuery;
struct addrinfo* serverAddressResultList;
fd_set read_fds;
struct timeval tv;
int socketId;
sockaddr* serverAddress;
AwgetRequest request;
char* saveFileName=NULL;

AwgetClient::AwgetClient(AwgetRequest awgetRequest) {
	request = awgetRequest;

	saveFileName = strrchr(request.url,'/') + 1;

	srand ( time(NULL) );
}


const char* AwgetClient::awget(){
	//get a random steppingstone from the list in the request.
	//SteppingStoneAddress ss = getRandomSteppingStoneAddressFromList(request.chainList, ntohs(request.chainListSize));
	SteppingStoneAddress ss = dequeRandomSteppingStoneAddressFromList(&request, ntohs(request.chainListSize));

	const char* fileLocation = requestAndSaveFile(ss, request);

	return fileLocation;
}

SteppingStoneAddress AwgetClient::dequeRandomSteppingStoneAddressFromList(AwgetRequest* awgetRequest, uint16_t size){
	//get random address...
	int randomIndex = rand() % size;
	SteppingStoneAddress returnAddress =  awgetRequest->chainList[randomIndex];

	debug("Next ss is <%s, %u> \n", returnAddress.hostAddress, ntohs(returnAddress.port));

	for(int i=randomIndex; i < (size - 1); i++)
	{
		awgetRequest->chainList[i] = awgetRequest->chainList[i+1];
	}

	awgetRequest->chainListSize = htons(size - 1);

	return returnAddress;
}


const char* AwgetClient::requestAndSaveFile(SteppingStoneAddress ss, AwgetRequest request){

	initializeConnection(ss);
	char inputBuffer[1024];
	bool dataComplete = false;
	//std::vector<unsigned char> fullOutput;
	int bytes;

	ofstream dataFile;
	dataFile.open(saveFileName, ios::out | ios::binary | ios::ate);


	bytes = send(socketId, (void *)&request, sizeof(struct AwgetRequest), 0);
	if(bytes < 0){
		perror("Unable to send data");
		exit(1);
	}

	info("waiting for file... \n");

	int status = select(socketId+1, &read_fds, NULL, NULL, &tv);

	if (status == FAILURE)
	{
		perror("select");
		exit(1);
	}


	 if (FD_ISSET(socketId, &read_fds))
	 {
		 while(!dataComplete)
		 {
			bytes = recv(socketId, (void *)&inputBuffer, sizeof(inputBuffer), 0);
			if(bytes == FAILURE)
			{
				perror("receive");
				exit(1);
			}

			if(bytes>0)
			{
				dataFile.write(inputBuffer, bytes);
			}
			else
			{
				dataComplete = true;
			}
		 }
	}
	else
	{
        error("Timeout occurred.\n");
        exit(1);
    }

	dataFile.close();
	close(socketId);

	debug("Received file %s \n", saveFileName);

	return (const char*)saveFileName;;
}

void AwgetClient::initializeConnection(SteppingStoneAddress ss){
	//open a tcp connection and send AwgetRequest to steppingstone
	//resolve the hostAddress from the steppingstone structure...
	int status;

	struct sockaddr_in sin;
	struct hostent *hp;

	hp = gethostbyname(ss.hostAddress);
	if(!hp){
		debug("Unknown host: %s\n.", ss.hostAddress);
		exit(1);
	}

	memset(&sin, 0x0000, sizeof(sin));
	memcpy(&sin.sin_addr, hp->h_addr_list[0], sizeof(hp->h_length));

	sin.sin_port = ss.port;
	sin.sin_family = AF_INET;

	socketId = socket(AF_INET, SOCK_STREAM, 0);

	 if(socketId < 0){
		perror("Unable to create socket \n");
		exit(1);
	 }

	 //int flags = fcntl(socketId, F_GETFL, 0);
	 //fcntl(socketId, F_SETFL, flags | O_NONBLOCK);

	//wait for response, will need a timeout here...
	 status = connect(socketId, (struct sockaddr *)&sin, sizeof(sin));

	 if(status == FAILURE){
		perror("unable to connect to server");
		exit(1);
	 }

	 FD_ZERO(&read_fds);
	 FD_SET(socketId,&read_fds);
	 tv.tv_sec = 3;
}





SteppingStoneAddress AwgetClient::getRandomSteppingStoneAddressFromList(SteppingStoneAddress steppingStones[], uint8_t size){
	/* initialize random seed: */
	//TODO: get it better..
	srand ( time(NULL) );
	int randomIndex = rand() % size;
	return steppingStones[randomIndex];
}


AwgetClient::~AwgetClient() {

}


