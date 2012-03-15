#include "../common/stepping_stone.h"
#include "../common/util.h"

using namespace std;

pthread_mutex_t requestDelegationHandlerMutext;

FileRetrieverService::FileRetrieverService()
{
	LOCAL_FILE_DIR = "/tmp/ss/";
	pthread_mutex_init(&requestDelegationHandlerMutext, NULL);
}

FileRetrieverService::~FileRetrieverService()
{
	pthread_mutex_destroy(&requestDelegationHandlerMutext);
}

void FileRetrieverService::handleRequest(AwgetRequest awgetRequest, int socketid)
{
	int randomIndex = (rand() % ntohs(awgetRequest.chainListSize));

	SteppingStoneAddress nextStone = awgetRequest.chainList[randomIndex];

	debug("Next SS is = <%s,%u> \n", nextStone.hostAddress, ntohs(nextStone.port));
	debug("Fetching %s \n", awgetRequest.url);

	pthread_mutex_lock(&requestDelegationHandlerMutext);

	prepareNewSSList(&awgetRequest, ntohs(awgetRequest.chainListSize), randomIndex);
	awgetRequest.chainListSize = htons(ntohs(awgetRequest.chainListSize) - 1);

	pthread_mutex_unlock(&requestDelegationHandlerMutext);

	ClientInterface clientInterface;
	clientInterface.retrieveFileFromNextSS(nextStone, awgetRequest, socketid);

}

void FileRetrieverService :: prepareNewSSList(AwgetRequest* awgetRequest, int arraySize, int itemIndexToRemove)
{
	for(int index = itemIndexToRemove;index < arraySize -1; index++)
	{
		awgetRequest->chainList[index] = awgetRequest->chainList[index + 1];
	}
}

void FileRetrieverService::wget(char* url, int socketid)
{
	string urlAsString = url;
	int position = urlAsString.find_last_of("/");
	string filename = urlAsString.substr(position);
    string fileLocation = getFileLocation(filename);

	string systemCommand = createWgetCommand(fileLocation, urlAsString);

	int status = system(systemCommand.c_str());

	if(status != 0)
	{
		throw "Unable to retrieve file using wget \n.";
	}

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
    	info("File received ..\n");
    	info("Relay file... \n");

    	fileStream.read(buffer, length);

    	int bytesSend = 0;
    	int bytesToBeSent = length;

    	while(bytesSend < length)
    	{
    		int bytes = send(socketid, &buffer, sizeof (buffer), 0);

    		if(bytes < 0)
    		{
    			throw "Unable to relay file";
    		}

    		bytesSend += bytes;
    		bytesToBeSent -=bytes;
    	}

    	info("Successful \n");
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
