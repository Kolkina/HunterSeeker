#ifndef __HUNTING_POLICY_H__
#define __HUNTING_POLICY_H__

#include "Policy.h"
#include <iostream>
#include <exception>
#include <stdlib.h>
#include <time.h>

#define eps 25

class HuntingPolicy: public Policy
{
	double _policyMap	[MAP_WIDTH][MAP_HEIGHT] // Self Position
						[MAP_WIDTH+1][MAP_HEIGHT+1] // Other Positon
						[MAP_WIDTH+1][MAP_HEIGHT+1] // Prey Position
						[ACTIONS];	
	int _prevX, _prevY, _prevAction;
	int _preyX, _preyY, _prevPreyX, _prevPreyY;
	int _otherX, _otherY, _prevOtherX, _prevOtherY;
	double alpha, gamma;
	
	int GetMaxAction();
	

	public:
		void PrintPolicy();
		HuntingPolicy();
		int GetAction();
		void SendObservation(int id, double value);


};

#endif