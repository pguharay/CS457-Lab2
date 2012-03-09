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
#include <netdb.h>

#include "../common/awget.h"


int main(int argc, char** argv)
{

	// check args
	char *chainFileName = NULL;
	char *fileURL = NULL;
	char *defaultChainFileName = "chaingang.txt";


	// == parse command line arguments ==
	int option;
	bool invalidArgs = false;
    int index;

	while ((option = getopt (argc, argv, "c:")) != -1)
		switch (c)
        {
           case 'c':
        	   chainFileName = optarg;
        	   break;

           case '?':
             if (optopt == 'c' )
               printf ("Option -%c requires an argument.\n", optopt);
             else
            	 invalidArgs = true;

          default:
             abort ();
        }

		if (invalidArgs || optind < 0 || optind > 1)
		{
			printf("\nUsage:  awget <URL> [-c chainfile] \n");
			return 1;
		}

		fileURL = argv[optind];

		// this for debug
	    printf ("URL = %s, chainfile = %s\n", fileURL, (chainFileName==NULL?"none":chainFileName));


	    // The chainfile argument is optional, and specifies the file containing information about
	    // the chain you want to use.  Since the chainfile argument is optional, if not specified
	    // awget should read the chain configuration from a local file called chaingang.txt.
	    // If no chainfile is given at the command line and awget fails to locate the chaingang.txt file,
	    // awget should print an error message and exit.


	    if (chainFileName != NULL)
	    {
	    	// check to make sure file exists

	    	// if so, read file

		    // if not or error reading, print error message and exit
	    }
	    else
	    {
	    	// check to see if default chain file exists
	    	// if so, read file

		    // if not or error reading, print error message and exit
	    }





	    // pick a random SS from the file to contact next.




	    // connect to SS and sends the URL and the chain list from the file, after stripping the selected entry.




	    // wait for the data to arrive




	    // save it into a local file - Note that the name of the file should be the one given in the URL (but not the entire URL).



	return 1;
}


