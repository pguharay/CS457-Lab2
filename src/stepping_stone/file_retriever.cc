#include <string>
#include <fstream>
#include "../common/stepping_stone.h"
#include "../common/util.h"

using namespace std;

void FileRetrieverService::handleRequest(AwgetRequest* awgetRequest, int socketid)
{
	int randomIndex = rand() % awgetRequest->chainListSize;
	SteppingStoneAddress nextStone = awgetRequest->chainList[randomIndex];

	debug("Next stepping stone = <%s>,<%u>", nextStone.hostAddress, nextStone.port);
}

string FileRetrieverService::getFileLocation(string filename)
{
    char currentThreadID[255];
    sprintf(currentThreadID, "%u", (unsigned long int)pthread_self());

    string fileLocation = LOCAL_FILE_DIR;
    fileLocation.append(currentThreadID);
    fileLocation.append("/");
    fileLocation.append(filename);

    return fileLocation;
}

string FileRetrieverService::createWgetCommand(string fileLocation, string urlAsString)
{
    string systemCommand = "wget --output-document ";
    systemCommand.append(fileLocation);
    systemCommand.append(" ");
    systemCommand.append(urlAsString);

    return systemCommand;
}

void FileRetrieverService::readFileAndStream(string fileLocation, int socketid)
{
    char buffer[255];
    int length = 255;
    ifstream fileStream;

    fileStream.open(fileLocation.c_str(), ios::in | ios::binary | ios::ate);

    if(fileStream.good() && fileStream.is_open())
    {
        while(!fileStream.eof())
        {
            fileStream.read(buffer, length);
            send(socketid, buffer, sizeof (buffer), 0);
        }
    }

    fileStream.close();
}

void FileRetrieverService::removeTemporaryFile(string fileLocation)
{
    string deleteCommand = "rm -f ";

    deleteCommand.append(fileLocation);

    debug("delete command [ %s ]", deleteCommand.c_str());

    system(deleteCommand.c_str());
}

void FileRetrieverService::wget(char* url, int socketid)
{
	string urlAsString = url;
	int position = urlAsString.find_last_of("\\");
	string filename = urlAsString.substr(position);
    string fileLocation = getFileLocation(filename);

	string systemCommand = createWgetCommand(fileLocation, urlAsString);

	debug("Command constructed = %s", systemCommand.c_str());

	system(systemCommand.c_str());

	readFileAndStream(fileLocation, socketid);

	removeTemporaryFile(fileLocation);
}


