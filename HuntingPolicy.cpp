
#include "HuntingPolicy.h"

HuntingPolicy::HuntingPolicy()
{
	
	for(int i = 0; i < MAP_WIDTH; i++) {
		for(int j = 0; j < MAP_HEIGHT; j++) {
			for(int k = 0; k < ACTIONS; k++) {
				_policyMap[i][j][k] = 0;
			}
		}
	}
	
	alpha = 0.9;
	gamma = 0.9;
	
}

int HuntingPolicy::GetMaxAction()
{
	int action = 0;
	
	for(int i = 0; i < ACTIONS; i++) {
		if(_policyMap[_posX][_posY][i] > _policyMap[_posX][_posY][action]) {
			action = i;
		}
	}
	
	return action;
}

int HuntingPolicy::GetAction()
{
	srand(time(NULL));
	int action = -1;
	if(rand() % 10 < 1) {
		action = rand() % ACTIONS;
	} else {
		action = GetMaxAction();
	}
	_prevAction = action;
	return action;
}

void HuntingPolicy::SendObservation(int id, double value)
{
	if(id == OSelfX) {
		_prevX = _posX;
		_posX = value;
	}
	else if(id == OSelfY) {
		_prevY = _posY;
		_posY = value;
	}
	else if(id == OReward) {
		_policyMap[_prevX][_prevY][_prevAction] += alpha*(value + gamma*_policyMap[_posX][_posY][GetMaxAction()] - _policyMap[_prevX][_prevY][_prevAction]);
	}
}
