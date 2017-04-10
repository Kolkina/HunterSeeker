
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
#if SIMPLE_COMM == true
		if(_policyMap[_posX][_posY][_otherX][_otherY][i] > _policyMap[_posX][_posY][_otherX][_otherY][action]) {
			action = i;
		}
#else
		if(_policyMap[_posX][_posY][i] > _policyMap[_posX][_posY][action]) {
			action = i;
		}
#endif
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

void SensingPolicy::printpolicy()
{
	std::cout << "COMMUNICATION POLICY MAP" << std::endl;
	std::cout << "{";
	for(int k = 0; k < COM_ACTIONS; k++) {
		std::cout << _policyMap[0][0][0][0][k] << ",";
	}
	std::cout << "}" << std::endl;
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
#if SIMPLE_COMM
		case OOtherX:
			_prevOtherX = _otherX;
			_otherX = val;
			break;
		case OOtherY:
			_prevOtherY = _otherY;
			_otherY = val;
			break;
#endif	
		case OReward:
#if SIMPLE_COMM
			_policyMap[_prevX][_prevY][_prevOtherX][_prevOtherY][_prevAction] += 	alpha*(val + gamma*
														_policyMap[_posX][_posY][_otherX][_otherY][GetMaxAction()] -
														_policyMap[_prevX][_prevY][_prevOtherX][_prevOtherY][_prevAction]);
#else
			_policyMap[_prevX][_prevY][_prevAction] += 	alpha*(val + gamma*
														_policyMap[_posX][_posY][GetMaxAction()] -
														_policyMap[_prevX][_prevY][_prevAction]);
#endif
			break;
		default:
			throw -1;
			break;
	}
}
