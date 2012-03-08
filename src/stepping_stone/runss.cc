#include "../common/stepping_stone.h"

int main(int argc, char** argv)
{
	SteppingStone* ss = new SteppingStone(8080);

	ss->start();

	return 1;
}




