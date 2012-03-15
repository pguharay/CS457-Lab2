#include "../common/client.h"


int main(int argc, char** argv)
{

	AwgetClient* client;
	//create a client argument..
	//clientArgument carg;
	//carg.documentUrl = "http://someurl/sometxt.txt";
	//carg.hostFile = "src/awget/chaingang.txt";

	SteppingStoneAddress ssa;
	strcpy(ssa.hostAddress, "JONDEV");
	//strcpy(ssa.hostAddress, "www.google.com");
	ssa.port = 80;

	AwgetRequest request;
	request.chainListSize=htonl(0);
	strcpy(request.url, "http://someurl/sometxt.txt");
	client->sendRequest(ssa, request);

}
