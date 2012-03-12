#include "../src/common/protocol.h"
#include "../src/common/util.h"

using namespace std;

AwgetRequest createRequest(int argc, char** argv)
{
	AwgetRequest awgetRequest;

	debug("URL %s \n", *(argv + 2));

	memcpy(awgetRequest.url,*(argv + 2), strlen(*(argv + 2)));

	awgetRequest.chainListSize = htons(2);

	SteppingStoneAddress firstStone, secondStone;

	strcpy(firstStone.hostAddress, "tomato.cs.colostate.edu");
	firstStone.port = htonl(8080);

	strcpy(secondStone.hostAddress, "spinach.cs.colostate.edu");
	secondStone.port = htonl(8080);

	awgetRequest.chainList[0] = firstStone;
	awgetRequest.chainList[1] = secondStone;

	return awgetRequest;
}

int connectSteppingStone(char* hostaddress, char* port)
{
	addrinfo hostAddressQuery;
	addrinfo* hostAddressResultList;

	debug("Stepping stone address [%s], Port [%s] \n", hostaddress, port);

	memset(&hostAddressQuery, 0x0000, sizeof(hostAddressQuery));

	hostAddressQuery.ai_family = AF_INET;
	hostAddressQuery.ai_socktype = SOCK_STREAM;

	int status = getaddrinfo(hostaddress, port, &hostAddressQuery, &hostAddressResultList);

	if(status != SUCCESS)
	{
		perror("Unable to get address information for stepping stone");
		exit(0);
	}

	int socketid = socket(AF_INET, SOCK_STREAM,0);

	if(socketid < 0)
	{
		perror("unable to create socket");
		exit(0);
	}

	status = connect(socketid, hostAddressResultList->ai_addr, hostAddressResultList->ai_addrlen);

	if(status == FAILURE)
	{
		perror("Unable to connect to stepping stone");
		exit(0);
	}

	return socketid;
}

void requestNextSSAndRelayResponse(AwgetRequest awgetRequest, int socketId)
{
	int bytes = send(socketId, (void*)&awgetRequest, sizeof(awgetRequest), 0);

	char response[MAX_FILE_SIZE];

	if(bytes != sizeof(awgetRequest))
	{
		perror("Failed to send request to server");
		exit(0);
	}

	ofstream fileStream;
	fileStream.open("/tmp/sstest/local.txt", ios::out);

	do
	{
		bytes = recv(socketId, (void*)response, MAX_FILE_SIZE, 0);

		if(bytes < 0)
		{
			perror("Unable to get response from server");
			exit(0);
		}

		debug("Received %d bytes \n", bytes);

		if(fileStream.is_open())
		{
			info("writing the content to local disk. \n");
			fileStream.write(response, bytes);
		}
	}while(bytes > 0);

	fileStream.close();

	close(socketId);
}

int main(int argc, char** argv)
{
	int socket = connectSteppingStone(*(argv + 4), *(argv + 6));
	requestNextSSAndRelayResponse(createRequest(argc, argv), socket);

	return 1;
}




