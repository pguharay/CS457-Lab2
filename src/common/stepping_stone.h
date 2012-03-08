#include "protocol.h"

#ifndef STEPPING_STONE_H_
#define STEPPING_STONE_H_

class SteppingStone
{
	private:
		void handleRequest(awgetRequest* request);
		void wget(char* url);
		uint8_t getNextStone(int chainListLength);
	public:
		SteppingStone(uint32_t port);
		virtual ~SteppingStone();
		virtual void start();
};


#endif /* STEPPING_STONE_H_ */
