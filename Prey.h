#ifndef __PREY_POLICY_H__
#define __PREY_POLICY_H__

#include "Policy.h"
#include <stdlib.h>
#include <time.h>

#define NUM_AGENTS 1

class Prey : public Policy
{
	Policy* agents[NUM_AGENTS];

	public:
		Prey(Policy*);
		int GetAction();
		void SendObservation(int id, int val);

};

#endif