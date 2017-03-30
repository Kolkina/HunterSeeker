#ifndef __SENSING_POLICY_H__
#define __SENSING_POLICY_H__

#include "Policy.h"

class SensingPolicy : public Policy
{

	double _policyMap[MAP_WIDTH][MAP_HEIGHT][ACTIONS];	
	int _prevX, _prevY, _prevAction;
	double alpha, gamma;

	public:
		int GetAction();
		void SendObservation(int id, double val);
		int GetCommunication();

};

#endif