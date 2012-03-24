#include "../common/stepping_stone.h"
#include "../common/util.h"

char* port;

int main(int argc, char** argv)
{
	try
	{
		validateStartArguments(argc, argv);
	}
	catch(const char* message)
	{
	    displayErrorAndExit(message);
	}

	SteppingStone* ss = new SteppingStone(port);

	ss->start();

	delete(ss);

	return 0;
}

void validateStartArguments(int argc, char** argv)
{
	if(argc == 3)
	{
		char* option = *(argv + 1);

		if (strcmp(option, "-p") != 0)
		{
			throw "Invalid launch option. \n";
		}

		port = *(argv + 2);

		if(!isNumeric(port))
		{
			throw "Port number must be integer between 1024 and 65536 \n";
		}

		if(startsWith(port, '0') && strlen(port) > 1)
		{
			throw "Port number must be integer between 1024 and 65536 \n";
		}

		uint32_t ssPort = atoi(port);

		if (ssPort < 1024 || ssPort > 65535)
		{
			throw "Port must be in range of 1024 to 65535. \n";
		}
	}
	else if(argc != 1)
	{
		throw "Invalid option \n";
	}
}

bool isNumeric(const char* value)
{
  int i=0;

  while(*(value + i) != '\0')
  {
    if(!std::isdigit(*(value + i)))
    {
      return false;
    }
    i++;
  }

  return true;
}

bool startsWith(char* value, char character)
{
	return *(value) == character;
}

void displayErrorAndExit(const char *message)
{
    error(message);
    error("usage: ss <optional-p port> \n");
    exit(1);
}



