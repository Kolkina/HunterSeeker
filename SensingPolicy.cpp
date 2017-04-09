
#include "SensingPolicy.h"

SensingPolicy::SensingPolicy()
{
	alpha = 0.7;
	gamma = 0.5;
}

int SensingPolicy::GetMaxAction()
{
	int action = 0;
	
	for(int i = 0; i < COM_ACTIONS; i++) {
		if(_policyMap[_posX][_posY][i] > _policyMap[_posX][_posY][action]) {
			action = i;
		}
	}
	
	return action;
}

int SensingPolicy::GetAction()
{	
	
	int action = -1;
	int rng = rand() % 100;
	rngness[rng]++;
	if(rng < eps) {
		int sel = rand() % COM_ACTIONS;
		while(sel == action) {
			sel = rand() % COM_ACTIONS;
		}
		action = sel;
	} else {
		action = GetMaxAction();
	}
	_prevAction = action;
	return action;
}

void SensingPolicy::SendObservation(int id, double val)
{
	switch(id) {
		case OPreyX:
			_prevX = _posX;
			_posX = val;
			break;
		case OPreyY:
			_prevY = _posY;
			_posY = val;
			break;
		case OReward:
			_policyMap[_prevX][_prevY][_prevAction] += 	alpha*(val + gamma*
														_policyMap[_posX][_posY][GetMaxAction()] -
														_policyMap[_prevX][_prevY][_prevAction]);
			break;
		default:
			throw -1;
			break;
	}
}
