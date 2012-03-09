#include "../common/stepping_stone.h"
#include "../common/util.h"

void FileRetrieverService::handleRequest(AwgetRequest* awgetRequest, int socketid)
{
	int randomIndex = rand() % awgetRequest->chainListSize;
	SteppingStoneAddress nextStone = awgetRequest->chainList[randomIndex];

	debug("Next stepping stone = <%s>,<%u>", nextStone.hostAddress, nextStone.port);
}

void FileRetrieverService::wget(char* url, int socketid)
{

}


