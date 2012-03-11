#include "../common/stepping_stone.h"
#include "../common/util.h"

using namespace std;


void ClientInterface :: retrieveFileFromNextSS(SteppingStoneAddress steppinStoneAddress, AwgetRequest* awgetRequest, int socketid)
{
	string nextStoneAddress = steppinStoneAddress.hostAddress;

	char port[10];
	sprintf(port, "%u", steppinStoneAddress.port);

	int clientSocketID = connectSteppingStone(nextStoneAddress.c_str(), port);

	requestNextSSAndRelayResponse(awgetRequest, clientSocketID, socketid);
}

void ClientInterface :: requestNextSSAndRelayResponse(AwgetRequest* awgetRequest, int clientSocketId, int serverSocketId)
{
	int bytes = send(clientSocketId, (void*)awgetRequest, sizeof(AwgetRequest), 0);

	char response[MAX_FILE_SIZE];

	if(bytes != sizeof(awgetRequest))
	{
		perror("Failed to send request to server");
		exit(0);
	}

	bytes = recv(clientSocketId, (void*)response, MAX_FILE_SIZE, 0);

	if(bytes < 0)
	{
		perror("Unable to get response from server");
		exit(0);
	}

	send(serverSocketId, response, sizeof(response), 0);
}

int ClientInterface :: connectSteppingStone(const char* hostaddress, char* port)
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
