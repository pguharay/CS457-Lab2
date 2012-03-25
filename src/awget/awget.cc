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

#include "../common/client.h"

void showUsageandExit()
{
	printf("\nUsage:  awget <URL> [-c chainfile] \n");
	exit(-1);
}

int main(int argc, char** argv)
{
	char* chainFileName = NULL;
	char defaultChainFileName[] = "chaingang.txt";

	char* fileURL = NULL;

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
		{
			fileURL = argv[i];
		}
	}


	// this for debug
//	printf ("URL = %s, chainfile = %s, save file name = %s\n", fileURL, chainFileName, saveFileName);


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

	strcpy(awgetRequest.url,fileURL);
	printf("Request: %s \n", awgetRequest.url);
	// read number of entries
	getline(chainFile, data);
	int numEntries = atoi(data.c_str());

	if (numEntries <= 0)
	{
		printf("Error reading chainfile: %s  (Invalid format)\n", chainFileName);
		return -1;
	}

	awgetRequest.chainListSize = htons(numEntries);

	/*
 	if (numEntries > MAX_HOSTS)
	{
		printf("Error reading chainfile: %s  (Max # entries exceeded)\n", chainFileName);
		return -1;
	}
	*/
	bool fileError = false;
	try
	{
		for (int i=0; i<numEntries; i++)
		{
			if (getline(chainFile, address, ','))
			{
				if (address.empty())
				{
					printf("Error reading chainfile: %s  (Invalid address)\n", chainFileName);
					fileError = true;
					break;
				}
			}
			else
			{
				printf("Error reading chainfile: %s \n", chainFileName);
				fileError = true;
				break;
			}

			if (getline(chainFile, data))
			{
				port = atoi(data.c_str());

				if (port < 1024 || port > 65535)
				{
					printf("Error reading chainfile: %s  (Invalid port)\n", chainFileName);
					fileError = true;
					break;
				}
			}
			else
			{
				printf("Error reading chainfile: %s \n", chainFileName);
				fileError = true;
				break;
			}

			// copy data to struct
			ssAddress entry;
			strcpy(entry.hostAddress,address.c_str());
			entry.port = htons(port);
			awgetRequest.chainList[i] = entry;
		}

		chainFile.close();
		if (fileError) return -1;

	}
	catch (...)
	{
		printf("Error reading chainfile: %s  (Invalid port)\n", chainFileName);
		return -1;
	}

	//printf("Chainfile %s has %i entries\n", chainFileName, ntohs(awgetRequest.chainListSize));
	printf("chainlist is\n");
	for (int i=0; i< ntohs(awgetRequest.chainListSize); i++)
		printf("%s, %i\n", awgetRequest.chainList[i].hostAddress, ntohs(awgetRequest.chainList[i].port));


	AwgetClient client(awgetRequest);
	const char* fileLocation = client.awget();

	if(fileLocation != NULL)
	{
		printf("\nFetched file successfully\n");
		printf("Result file: ./%s\n", fileLocation);
	}
	else
	{
		printf("Unable to retrieve file successfully \n");
	}

	printf("Goodbye!");

	return 1;
}



