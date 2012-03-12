/*
 * awget.cc
 *
 *  Created on: Mar 8, 2012
 *      Author: rebecca
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <fstream>
using namespace std;


#include "../common/protocol.h"
#include "../common/stepping_stone.h"

void showUsageandExit()
{
	printf("\nUsage:  awget <URL> [-c chainfile] \n");
	exit(-1);
}

int main(int argc, char** argv)
{
	char* chainFileName = NULL;
	char* fileURL = NULL;
	char defaultChainFileName[] = "chaingang.txt";


	// == parse command line arguments ==
	if (argc != 2 && argc != 4)
		showUsageandExit();

	for (int i=1; i<argc; i++)
	{
		if ( strcmp(argv[i], "-c") == 0 )
		{
			chainFileName = argv[i+1];
			if (chainFileName == NULL)
				showUsageandExit();
			i++;
		}
		else if (argv[i][0]== '-')
			showUsageandExit();
		else
			fileURL = argv[i];
	}

	// this for debug
	printf ("URL = %s, chainfile = %s\n", fileURL, chainFileName);


	// The chainfile argument is optional, and specifies the file containing information about
	// the chain you want to use.  Since the chainfile argument is optional, if not specified
	// awget should read the chain configuration from a local file called chaingang.txt.
	// If no chainfile is given at the command line and awget fails to locate the chaingang.txt file,
	// awget should print an error message and exit.


	if (chainFileName == NULL)
		chainFileName = defaultChainFileName;

    ifstream chainFile(chainFileName);

    if (!chainFile.is_open())
    {
    	printf("Error reading chainfile: %s  (File does not exist)\n", chainFileName);
    	return -1;
    }

    string data;
	string address;
	int port;

	AwgetRequest awgetRequest;
//	awgetRequest.url = NULL;


	// read number of entries
	getline(chainFile, data);
	int numEntries = atoi(data.c_str());

	if (numEntries <= 0)
	{
		printf("Error reading chainfile: %s  (Invalid format)\n", chainFileName);
		return -1;
	}

	awgetRequest.chainListSize = numEntries;

	/*
 	if (numEntries > MAX_HOSTS)
	{
		printf("Error reading chainfile: %s  (Max # entries exceeded)\n", chainFileName);
		return -1;
	}
	*/

	try
	{
		for (int i=0; i<numEntries; i++)
		{
			if (getline(chainFile, address, ','))
			{
				if (address.empty())
				{
					printf("Error reading chainfile: %s  (Invalid address)\n", chainFileName);
					return -1;
				}
			}
			else
			{
				printf("Error reading chainfile: %s \n", chainFileName);
				return -1;
			}

			if (getline(chainFile, data))
			{
				port = atoi(data.c_str());

				if (port < 1024 || port > 65535)
				{
					printf("Error reading chainfile: %s  (Invalid port)\n", chainFileName);
					return -1;
				}
			}
			else
			{
				printf("Error reading chainfile: %s \n", chainFileName);
				return -1;
			}

			// copy data to struct
			ssAddress entry;
			strcpy(entry.hostAddress,address.c_str());
			entry.port = port;
			awgetRequest.chainList[i] = entry;
		}
	}
	catch (...)
	{
		printf("Error reading chainfile: %s  (Invalid port)\n", chainFileName);
		return -1;
	}

	// for testing - verify file read
	printf("%s has %i entries\n", chainFileName, awgetRequest.chainListSize);
	for (int i=0; i< awgetRequest.chainListSize; i++)
		printf("%s, %i\n", awgetRequest.chainList[i].hostAddress, awgetRequest.chainList[i].port);



	// === set up FileRetrieverService and send request ===

//	FileRetrieverService service;
//	int socketID = 6000;
//	service.handleRequest(&awgetRequest, socketID);


	// === set up listener and wait for the data to arrive


	// save it into a local file - Note that the name of the file should be the one given in the URL (but not the entire URL).


	return 1;
}



