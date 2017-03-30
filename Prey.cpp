
#include "Prey.h"

Prey::Prey() {}

Prey::Prey(Policy* agent[])
{
	for(int i = 0; i < NUM_AGENTS; i++) {
		agents[i] = agent[i];
	}
}

int Prey::GetAction()
{
	srand(time(NULL));
	int action = rand() % 4;
	for(int i = 0; i < NUM_AGENTS; i++) {
		if(abs(agents[i]->_posX - _posX) < DETECT_RANGE && abs(agents[i]->_posY - _posY) < DETECT_RANGE) {
			if(agents[i]->_posX - _posX <= 0 && _posX < MAP_WIDTH-1) {
				// Agent is left
				action = ARight;
			} else if(agents[i]->_posX - _posX >= 0 && _posX > 0) {
				// Agent is right
				action = ALeft;
			} else if(agents[i]->_posY - _posY <= 0 && _posY < MAP_HEIGHT-1) {
				// Agent is up
				action = ADown;
			} else if(agents[i]->_posY - _posY >= 0 && _posY > 0) {
				// Agent is down
				action = AUp;
			}
		}
	}
	return action;
}

void Prey::SendObservation(int id, double value)
{
	if(id == OSelfX) {
		_posX = value;
	}
	else if(id == OSelfY) {
		_posY = value;
	}
}