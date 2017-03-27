#ifndef __PREY_POLICY_H__
#define __PREY_POLICY_H__

#include "Policy.h"
#include <stdlib.h>

#define NUM_AGENTS 2

class Prey : public Policy
{
	Policy agents[NUM_AGENTS];

	public:
		int GetAction();
		void SendObservation(int id, int val);

};

#endif