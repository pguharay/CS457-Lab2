
char* documentUrl;
char* ssFileLocation;

int main(int argc, char** argv)
{
	try
	{
		parseArguments(argc, argv);
	}
	catch(const char* message)
	{
	    displayErrorAndExit(message);
	}


	return 1;
}


void parseArguments(int argc, char** argv){

	 while ((c = getopt (argc, argv, "c:")) != -1)
	         switch (c)
	           {
	           case 'c':
	             ssFileLocation = optarg;
	             break;
	           default:
	             abort();
	           }

	  for (index = optind; index < argc; index++)
		  documentUrl = argv[index];

}






