#include "../common/stepping_stone.h"
#include "../common/util.h"

using namespace std;

FileRetrieverService::FileRetrieverService()
{
	LOCAL_FILE_DIR = "/tmp/ss/";
}

void FileRetrieverService::handleRequest(AwgetRequest* awgetRequest, int socketid)
{
	int randomIndex = (rand() % ntohs(awgetRequest->chainListSize));

	debug("Random index %d \n", randomIndex);

	SteppingStoneAddress nextStone = awgetRequest->chainList[randomIndex];

	debug("Next stepping stone = <%s>,<%u>", nextStone.hostAddress, ntohl(nextStone.port));

	awgetRequest->chainListSize = ntohs(awgetRequest->chainListSize) - 1;
	SteppingStoneAddress newChainList[ntohs(awgetRequest->chainListSize)];

	prepareNewSSList(awgetRequest->chainList, newChainList, (ntohs(awgetRequest->chainListSize) +1), randomIndex);

	memcpy(&newChainList, awgetRequest->chainList,ntohs(awgetRequest->chainListSize));

	ClientInterface clientInterface;
	clientInterface.retrieveFileFromNextSS(nextStone, awgetRequest, socketid);
}

void FileRetrieverService :: prepareNewSSList(SteppingStoneAddress oldChainList[], SteppingStoneAddress newChainList[], int arraySize, int itemIndexToRemove)
{
	int index = 0;

	for(int i = 0;i < arraySize; i++)
	{
		if(i != itemIndexToRemove)
		{
			newChainList[index++] = oldChainList[i];
		}
	}
}

void FileRetrieverService::wget(char* url, int socketid)
{
	string urlAsString = url;
	int position = urlAsString.find_last_of("/");
	string filename = urlAsString.substr(position);
    string fileLocation = getFileLocation(filename);

	string systemCommand = createWgetCommand(fileLocation, urlAsString);

	system(systemCommand.c_str());

	readFileAndStream(fileLocation, socketid);

	removeTemporaryFile(fileLocation);
}

void FileRetrieverService::createTemporaryDirectory(string & fileLocation)
{
    string createdDirCommand = "mkdir -p ";
    createdDirCommand.append(fileLocation);
    system(createdDirCommand.c_str());
}

string FileRetrieverService::getFileLocation(string filename)
{
    char currentTaskID[255];

    sprintf(currentTaskID, "%u", rand());

    string fileLocation = LOCAL_FILE_DIR;
    fileLocation.append(currentTaskID);

    createTemporaryDirectory(fileLocation);

    fileLocation.append(filename);

    return fileLocation;
}

string FileRetrieverService::createWgetCommand(string fileLocation, string urlAsString)
{
    string systemCommand = "wget -q --output-document ";
    systemCommand.append(fileLocation);
    systemCommand.append(" ");
    systemCommand.append(urlAsString);

    return systemCommand;
}

void FileRetrieverService::readFileAndStream(string fileLocation, int socketid)
{
    ifstream fileStream;

    fileStream.open(fileLocation.c_str(), ios::in | ios::binary);

    fileStream.seekg(0, ios::end);

    int length = fileStream.tellg();
    char buffer[length];

    fileStream.seekg(0, ios::beg);

    if(fileStream.good() && fileStream.is_open())
    {
    	info("Realy file... \n");
    	fileStream.read(buffer, length);
    	send(socketid, &buffer, sizeof (buffer), 0);
    	info("Success \n");
    }

    fileStream.close();
}

void FileRetrieverService::removeTemporaryFile(string fileLocation)
{
    string deleteCommand = "rm -rf ";

    int lastIndex = fileLocation.find_last_of("/");

    string fileDir = fileLocation.substr(0,lastIndex);

    deleteCommand.append(fileDir);

    system(deleteCommand.c_str());
}
