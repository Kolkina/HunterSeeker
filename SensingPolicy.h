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
#define SIMPLE_COMM true


enum Communication
{
	CNothing,
	CPreyX,
	CPreyY,
	CPreyXY,
};

class SensingPolicy : public Policy
{

#if SIMPLE_COMM == true
	double _policyMap 	[MAP_HEIGHT][MAP_WIDTH] // Prey Position
						[MAP_HEIGHT][MAP_WIDTH] // Hunter Position
						[COM_ACTIONS];
#else
	double _policyMap[SENSING_WIDTH][SENSING_HEIGHT][COM_ACTIONS];
#endif
	int _prevX, _prevY, _prevAction;
#if SIMPLE_COMM == true
	int _otherX, _otherY, _prevOtherX, _prevOtherY;
#endif
	
	double alpha, gamma;

	int GetMaxAction();
	
	public:
		SensingPolicy();
		void printpolicy();
		
		int rngness[100];
		int eps = 10;
		int GetAction();
		void SendObservation(int id, double val);

};

#endif