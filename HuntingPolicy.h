#ifndef __HUNTING_POLICY_H__
#define __HUNTING_POLICY_H__

#include "Policy.h"
#include <iostream>
#include <exception>
#include <stdlib.h>
#include <time.h>

class HuntingPolicy: public Policy
{
	double _policyMap[MAP_WIDTH][MAP_HEIGHT][ACTIONS];	
	int _prevX, _prevY, _prevAction;
	double alpha, gamma;
	
	int GetMaxAction();
	

	public:
		HuntingPolicy();
		int GetAction();
		void SendObservation(int id, int value);
		void SendReward(double value);


};

#endif