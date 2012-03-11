/*
 * client.cpp
 *
 *  Created on: Mar 8, 2012
 *      Author: jon
 */
#include "../common/client.h"


clientArgument args;
struct addrinfo  serverAddressQuery;
struct addrinfo* serverAddressResultList;
fd_set read_fds;
struct timeval tv;


client::client(clientArgument* clientArg) {
	args = clientArg;
}

char* sendRequest(AwgetRequest* request){
	char* inputBuffer[1024];
	bool dataComplete = false;
	string fullOutput;
	//get a random steppingstone from the list in the request.
	SteppingStoneAddress ss = getRandomSteppingStoneAddressFromList(request->chainList, request->chainListSize);
	initializeConnection(ss);
	int bytes;
	socklen_t addrlen = sizeof(struct sockaddr);
	bytes = send(socketid, request, sizeof(struct AwgetRequest), 0);
	if(bytes < 0){
		perror("Unable to send data");
		exit(1);
	}

	int status = select(socketid+1, &read_fds, NULL, NULL, &tv);
	if (status == FAILURE) {
		perror("select");
		exit(1);
	}

	 if (FD_ISSET(socketid, &read_fds)){
		 while(!dataComplete){
			bytes = recv(s, (void *)&inputBuffer, sizeof(inputBuffer), 0);
			if(bytes == FAILURE){
				perror("receive");
				exit(1);
			}

			if(bytes>0){
				fullOutput.append(inputBuffer, bytes);
			}else{
				dataComplete = true;
			}
		 }
	}
	else{
        error("Timeout occurred.\n");
        exit(1);
    }

	close(socketid);
	return fullOutput.data();
}

void initializeConnection(SteppingStoneAddress ss){
	//open a tcp connection and send AwgetRequest to steppingstone
	//resolve the hostAddress from the steppingstone structure...
	memset(&serverAddressQuery, 0x0000, sizeof(serverAddressQuery));
	serverAddressQuery.ai_family = AF_INET;
	serverAddressQuery.ai_socktype = SOCK_STREAM;

	int status = getaddrinfo(ss.hostAddress,ss.port, &serverAddressQuery, &serverAddressResultList);

	if(status != SUCCESS){
		error("Unable to get server host information \n");
		exit(1);
	}

	serverAddress = serverAddressResultList->ai_addr;

	socketid = socket(serverAddressResultList->ai_family, serverAddressResultList->ai_socktype, 0);

	 if(socketid < 0){
		perror("Unable to create socket \n");
		exit(1);
	 }

	//wait for response, will need a timeout here...
	 status = connect(socketid, serverAddress, serverAddressResultList->ai_addrlen);
	 if(status == FAILURE){
		perror("unable to connect to server");
		exit(1);
	 }

	 FD_ZERO(&read_fds);
	 FD_SET(socketid,&read_fds);
	 tv.tv_sec = 3;
}


SteppingStoneAddress getRandomSteppingStoneAddressFromList(SteppingStoneAddress steppingStones[], uint8_t size){
	/* initialize random seed: */
	srand ( time(NULL) );
	int randomIndex = rand() % size;
	return steppingStones[randomIndex];
}

client::~client() {

}


