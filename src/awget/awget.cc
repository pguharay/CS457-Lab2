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


int main(int argc, char** argv)
{

	// check args
	char *chainFileName = NULL;
	char *fileURL = NULL;


	// == parse command line arguments ==
	int option;
	bool invalidArgs = false;
    int index;

	while ((option = getopt (argc, argv, "x:t:s:p:")) != -1)
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

		// for debug
	    printf ("URL = %s, chainfile = %s\n", fileURL, (chainFileName==NULL?"none":chainFileName));

	return 1;
}

