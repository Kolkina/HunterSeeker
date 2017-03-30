#ifndef __POLICY_H__
#define __POLICY_H__

#include <vector>
#include <string>

#define MAP_WIDTH 10
#define MAP_HEIGHT 10
#define ACTIONS 4

enum Action
{
	AUp,
	ADown,
	ALeft,
	ARight
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