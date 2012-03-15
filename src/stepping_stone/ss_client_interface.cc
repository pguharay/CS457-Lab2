#include "../common/stepping_stone.h"
#include "../common/util.h"

using namespace std;


void ClientInterface :: retrieveFileFromNextSS(SteppingStoneAddress steppinStoneAddress, AwgetRequest awgetRequest, int socketid)
{
	string nextStoneAddress = steppinStoneAddress.hostAddress;

	char port[10];
	sprintf(port, "%d", ntohl(steppinStoneAddress.port));

	int clientSocketID = connectSteppingStone(nextStoneAddress.c_str(), port);

	requestNextSSAndRelayResponse(awgetRequest, clientSocketID, socketid);
}

void ClientInterface :: requestNextSSAndRelayResponse(AwgetRequest awgetRequest, int clientSocketId, int serverSocketId)
{
	int bytes = send(clientSocketId, (void*)&awgetRequest, sizeof(AwgetRequest), 0);
	info("Waiting for the file ... \n");

	char response[MAX_FILE_SIZE];

	if(bytes != sizeof(AwgetRequest))
	{
		throw "Failed to send request to server \n";
	}

	info("Relaying file...  \n");

	do
	{
		bytes = recv(clientSocketId, (void*)response, MAX_FILE_SIZE, 0);

		if(bytes < 0)
		{
			throw "Unable to get response from server \n";
		}

		if(bytes > 0)
		{
			send(serverSocketId, response, bytes, 0);
		}

	}while(bytes > 0);

	close(clientSocketId);
	close(serverSocketId);

	info("Successful. \n");
}

int ClientInterface :: connectSteppingStone(const char* hostaddress, char* port)
{
	addrinfo hostAddressQuery;
	addrinfo* hostAddressResultList;

	memset(&hostAddressQuery, 0x0000, sizeof(hostAddressQuery));

	hostAddressQuery.ai_family = AF_INET;
	hostAddressQuery.ai_socktype = SOCK_STREAM;

	int status = getaddrinfo(hostaddress, port, &hostAddressQuery, &hostAddressResultList);

	if(status != SUCCESS)
	{
		throw "Unable to get address information for stepping stone \n";
	}

	int socketid = socket(AF_INET, SOCK_STREAM,0);

	if(socketid < 0)
	{
		throw "unable to create socket";
	}

	status = connect(socketid, hostAddressResultList->ai_addr, hostAddressResultList->ai_addrlen);

	if(status == FAILURE)
	{
		throw "Unable to connect to stepping stone \n";
	}

	return socketid;
}
