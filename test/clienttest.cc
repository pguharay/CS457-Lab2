#include "../src/common/protocol.h"
#include "../src/common/util.h"
#include "../src/common/client.h"


int main(int argc, char** argv)
{
	//create a client argument..
	clientArgument carg;
	carg.documentUrl = "http://someurl/sometxt.txt";
	carg.hostFile = "chaingang.txt";

	//create a new client
	Client client = new Client(carg);

	client.awget();
}
