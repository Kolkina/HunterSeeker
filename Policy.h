#ifndef __POLICY_H__
#define __POLICY_H__

#include <vector>
#include <string>

#define MAP_WIDTH 4
#define MAP_HEIGHT 4
#define ACTIONS 5
#define GOAL_REWARD 500

enum Action
{
	AUp = 0,
	ADown = 1,
	ALeft = 2,
	ARight = 3,
	ANothing = 4,
};

enum Observation
{
	OSelfX,
	OSelfY,
	OOtherX,
	OOtherY,
	OPreyX,
	OPreyY,
	OReward,
	OObsReward,
};


class Policy
{
	public:
		int _posX, _posY;
		Policy() {_posX = -1; _posY = -1;}
		
	public:
		virtual int GetAction() = 0;
		virtual void SendObservation(int id, double val) = 0;

};

#endif