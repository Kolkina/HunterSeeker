
#include "HuntingPolicy.h"

HuntingPolicy::HuntingPolicy() : _preyX(-1), _preyY(-1), _otherX(-1), _otherY(-1)
{
	
	for(int i = 0; i < MAP_WIDTH*MAP_WIDTH*MAP_WIDTH; i++) {
		for(int j = 0; j < MAP_HEIGHT*MAP_HEIGHT*MAP_HEIGHT; j++) {
			for(int k = 0; k < ACTIONS; k++) {
				_policyMap[i/(MAP_WIDTH*MAP_WIDTH)][j/(MAP_HEIGHT*MAP_HEIGHT)][i/MAP_WIDTH % MAP_WIDTH][j/MAP_HEIGHT % MAP_HEIGHT][i % MAP_WIDTH][j % MAP_HEIGHT][k] = 0;
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
		if(_policyMap[_posX][_posY][_otherX][_otherY][_preyX][_preyY][i] > _policyMap[_posX][_posY][_otherX][_otherY][_preyX][_preyY][action]) {
			action = i;
		}
	}
	
	return action;
}

int HuntingPolicy::GetAction()
{
	srand(time(NULL));
	int action = -1;
	if(rand() % 100 < eps) {
		int sel = rand() % ACTIONS;
		while(sel == action) {
			sel = rand() % ACTIONS;
		}
		action = sel;
	} else {
		action = GetMaxAction();
	}
	_prevAction = action;
	return action;
}

void HuntingPolicy::PrintPolicy()
{
	for(int i = 0; i < MAP_WIDTH*MAP_WIDTH*MAP_WIDTH; i++) {
		for(int j = 0; j < MAP_HEIGHT*MAP_HEIGHT*MAP_HEIGHT; j++) {
			for(int k = 0; k < ACTIONS; k++) {
				std::cout << _policyMap[i/(MAP_WIDTH*MAP_WIDTH)][j/(MAP_HEIGHT*MAP_HEIGHT)][i/MAP_WIDTH % MAP_WIDTH][j/MAP_HEIGHT % MAP_HEIGHT][i % MAP_WIDTH][j % MAP_HEIGHT][k] << std::endl;
			}
		}
	}
}

void HuntingPolicy::SendObservation(int id, double value)
{

	switch(id) {
		case OSelfX :
			_prevX = _posX;
			_posX = value;
			break;
	
		case OSelfY :
			_prevY = _posY;
			_posY = value;
			break;
	
		case OOtherX :
			_prevOtherX = _otherX;
			_otherX = value;
			break;
			
		case OOtherY :
			_prevOtherY = _otherY;
			_otherY = value;
			break;
	
		case OPreyX :
			_prevPreyX = _preyX;
			_preyX = value;
			break;
	
		case OPreyY :
			_prevPreyY = _preyY;
			_preyY = value;
			break;
	
		case OReward :
			_policyMap[_prevX][_prevY][_prevOtherX][_prevOtherY][_prevPreyX][_prevPreyY][_prevAction] += 	
																	alpha*(value + gamma*
																	_policyMap[_posX][_posY][_otherX][_otherY][_preyX][_preyY][GetMaxAction()] -
																	_policyMap[_prevX][_prevY][_prevOtherX][_prevOtherY][_prevPreyX][_prevPreyY][_prevAction]);
			if(_policyMap[_prevX][_prevY][_prevOtherX][_prevOtherY][_prevPreyX][_prevPreyY][_prevAction] > 1000) _policyMap[_prevX][_prevY][_prevOtherX][_prevOtherY][_prevPreyX][_prevPreyY][_prevAction] = -1e9;
			break;
	}
}
