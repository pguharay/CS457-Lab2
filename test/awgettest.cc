#include "../src/common/protocol.h"
#include "../src/common/util.h"
#include "../src/common/client.h"


int main(int argc, char** argv)
{
	AwgetClient* client;
	//create a client argument..
	clientArgument carg;
	carg.documentUrl = "http://someurl/sometxt.txt";
	carg.hostFile = "src/awget/chaingang.txt";

	//create a new client
	client = new AwgetClient(carg);
	client->awget();

}


