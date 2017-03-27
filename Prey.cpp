
#include "Prey.h"

int Prey::GetAction()
{
	int minDist = 10000;
	for(int i = 0; i < NUM_AGENTS; i++) {
		if(std::abs(agents[i]._posX - _posX) < 2 && std::abs(agents[i]._posY - _posY) < 2) {

		}
	}

}

void Prey::SendObservation(int id, int val)
{

}