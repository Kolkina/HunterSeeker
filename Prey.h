#ifndef __PREY_POLICY_H__
#define __PREY_POLICY_H__

#include "Policy.h"
#include <stdlib.h>
#include <time.h>

#define NUM_AGENTS 1
#define DETECT_RANGE 3

class Prey : public Policy
{
	Policy* agents[NUM_AGENTS];

	public:
		Prey();
		Prey(Policy*[]);
		int GetAction();
		void SendObservation(int id, double val);

};

#endif