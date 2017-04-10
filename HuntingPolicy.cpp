
#include "HuntingPolicy.h"

HuntingPolicy::HuntingPolicy() : _preyX(-1), _preyY(-1), _otherX(-1), _otherY(-1)
{
	for(int i = 0; i < MAP_WIDTH*MAP_WIDTH*MAP_WIDTH; i++) {
		for(int j = 0; j < MAP_HEIGHT*MAP_HEIGHT*MAP_HEIGHT; j++) {
			for(int k = 0; k < ACTIONS; k++) {
				_policyMap	[i/(MAP_WIDTH*MAP_WIDTH)][j/(MAP_HEIGHT*MAP_HEIGHT)]
							[i/MAP_WIDTH % MAP_WIDTH][j/MAP_HEIGHT % MAP_HEIGHT]
							[i % MAP_WIDTH][j % MAP_HEIGHT]
							[k] = 0;
			}
		}
	}
	
	alpha = 0.7;
	gamma = 0.5;
	
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
	if(_posX == 0 && _posY == 0 && _preyX == 0 && _preyY == 1 && action == ADown && !isSensor && !firstNext) {
		std::cout << "Next to prey:\t\t";
		std::cout << "{";
		for(int k = 0; k < ACTIONS; k++) {
			std::cout << _policyMap[0][0][4][4][0][1][k] << ", ";
		}
		std::cout << "}" << std::endl;
		wasNext = true;
		firstNext = true;
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
			if(_prevX == 0 && _prevY == 0 && _prevPreyX == 0 && _prevPreyY == 1 && _prevAction == ADown && !firstNext && !wasNext) {
				std::cout << "{";
				for(int k = 0; k < ACTIONS; k++) {
					std::cout << _policyMap[0][0][4][4][0][1][k] << ", ";
				}
				std::cout << "}" << std::endl;
				throw -1;
			}
			_policyMap[_prevX][_prevY][_prevOtherX][_prevOtherY][_prevPreyX][_prevPreyY][_prevAction] += 	
																	alpha*(value + gamma*
																	_policyMap[_posX][_posY][_otherX][_otherY][_preyX][_preyY][GetMaxAction()] -
																	_policyMap[_prevX][_prevY][_prevOtherX][_prevOtherY][_prevPreyX][_prevPreyY][_prevAction]);
																	
			if(_policyMap[_prevX][_prevY][_prevOtherX][_prevOtherY][_prevPreyX][_prevPreyY][_prevAction] > GOAL_REWARD*2) _policyMap[_prevX][_prevY][_prevOtherX][_prevOtherY][_prevPreyX][_prevPreyY][_prevAction] = -1e9;
			if(wasNext && value > 0) {
				std::cout << "New policy\t\t";
				std::cout << "{";
				for(int k = 0; k < ACTIONS; k++) {
					std::cout << _policyMap[0][0][4][4][0][1][k] << ", ";
				}
				std::cout << "}" << std::endl;
				std::cout << "Currents:" << std::endl;
				std::cout << "\t(PosX,PosY):\t(" << _posX << "," << _posY << ")" << std::endl;
				std::cout << "\t(PreyX,PreyY):\t(" << _preyX << "," << _preyY << ")" << std::endl;
				std::cout << "Previous:" << std::endl;
				std::cout << "\t(PosX,PosY):\t(" << _prevX << "," << _prevY << ")" << std::endl;
				std::cout << "\t(PreyX,PreyY):\t(" << _prevPreyX << "," << _prevPreyY << ")" << std::endl;
				wasNext = false;
			}
			break;
	}
}














