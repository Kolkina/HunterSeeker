#ifndef __SENSING_POLICY_H__
#define __SENSING_POLICY_H__

#include "Policy.h"
#include <random>
#include <stdlib.h>
#include <time.h>
#include <iostream>

#define SENSING_WIDTH 5
#define SENSING_HEIGHT 5
#define COM_ACTIONS 4


enum Communication
{
	CNothing,
	CPreyX,
	CPreyY,
	CPreyXY,
};

class SensingPolicy : public Policy
{

	double _policyMap[SENSING_WIDTH][SENSING_HEIGHT][COM_ACTIONS];
	int _prevX, _prevY, _prevAction;
	double alpha, gamma;

	int GetMaxAction();
	
	public:
		SensingPolicy();
		
		int rngness[100];
		int eps = 10;
		int GetAction();
		void SendObservation(int id, double val);

};

#endif