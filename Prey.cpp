
#include "Prey.h"

Prey::Prey(Policy* agent)
{
	agents[0] = agent;
}

int Prey::GetAction()
{
	srand(time(NULL));
	int action = rand() % 4;
	for(int i = 0; i < NUM_AGENTS; i++) {
		if(abs(agents[i]->_posX - _posX) < 2 && abs(agents[i]->_posY - _posY) < 2) {
			if(agents[i]->_posX - _posX <= 0 && _posX > 0) {
				// Agent is left
				action = ALeft;
			} else if(agents[i]->_posX - _posX >= 0 && _posX < MAP_WIDTH-1) {
				// Agent is right
				action = ARight;
			} else if(agents[i]->_posY - _posY <= 0 && _posY < MAP_HEIGHT-1) {
				// Agent is up
				action = ADown;
			} else if(agents[i]->_posY - _posY >= 0 && _posY > 0) {
				// Agent is down
				action = ADown;
			}
		}
	}
	return action;
}

void Prey::SendObservation(int id, int value)
{
	if(id == OSelfX) {
		_posX = value;
	}
	else if(id == OSelfY) {
		_posY = value;
	}
}