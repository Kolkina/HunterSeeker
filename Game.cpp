
#include "Policy.h"
#include "HuntingPolicy.h"
#include "SensingPolicy.h"
#include "Prey.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>

#define TARGET_GAMES 5000

// Game Values
int _map[MAP_WIDTH][MAP_HEIGHT];
int _gamesPlayed, _totalReward;
int rewards[50];

// Agents
HuntingPolicy _hunter;
HuntingPolicy _sensor;
Prey prey;

enum Objects
{
	UNothing,
	UAgent,
	UGoal
};


void init()
{
	_gamesPlayed = 0;
	_totalReward = 0;

	for(int i = 0; i < MAP_WIDTH; i++) {
		for(int j = 0; j < MAP_HEIGHT; j++) {
			_map[i][j] = UNothing;
		}	
	}
	
	_map[1][1] = UAgent;
	_map[5][5] = UGoal;
	_map[9][9] = UAgent;
	
	//_sensor.GetAction();
	
	_hunter.SendObservation(OSelfX,1);
	_hunter.SendObservation(OSelfY,1);
	_sensor.SendObservation(OSelfX,9);
	_sensor.SendObservation(OSelfY,9);
	_sensor.SendObservation(OOtherX, MAP_WIDTH);
	_sensor.SendObservation(OOtherY, MAP_HEIGHT);
	_sensor.SendObservation(OOtherX, MAP_WIDTH);
	_sensor.SendObservation(OOtherY, MAP_HEIGHT);
	_sensor.SendObservation(OPreyX, MAP_WIDTH);
	_sensor.SendObservation(OPreyY, MAP_HEIGHT);
	_sensor.SendObservation(OPreyX, MAP_WIDTH);
	_sensor.SendObservation(OPreyY, MAP_HEIGHT);
	Policy* l[2];
	l[0] = &_hunter;
	l[1] = &_sensor;
	prey = Prey(l);
	prey.SendObservation(OSelfX,5);
	prey.SendObservation(OSelfY,5);

}

void moveagent(Policy& agent, int newX, int newY)
{
	// Store Temps
	int preX = agent._posX;
	int preY = agent._posY;
	
	// Move Agent
	agent.SendObservation(OSelfX,newX);
	agent.SendObservation(OSelfY,newY);
	
	// Swap Tiles
	int preV = _map[preX][preY];
	_map[preX][preY] = _map[newX][newY];
	_map[newX][newY] = preV;
	
}

void getcheckvalues(int& xcheck, int& ycheck, int action)
{
	if(action == AUp) {
		ycheck -= 1;
	} else if(action == ADown) {
		ycheck += 1;
	} else if(action == ALeft) {
		xcheck -= 1;
	} else if(action == ARight) {
		xcheck += 1;
	}
}

#define DISPLAY_MOVES false
#define TELL_PREY true
#define TELL_OTHER false
#define SPLIT_REWARD false

int performactions(int& loops, bool& done)
{
	int action = prey.GetAction();
	
	int xToCheck = prey._posX, yToCheck = prey._posY;
	
	if(_gamesPlayed == 500 && DISPLAY_MOVES) {
		std::cout << "======" << std::endl;
		std::cout << "Prey:" << prey._posX << ":" << prey._posY << std::endl;
		std::cout << "Prey Action:" << action << std::endl;
	}		
	getcheckvalues(xToCheck, yToCheck, action);
	
	
	if(yToCheck < 0 || yToCheck >= MAP_HEIGHT || xToCheck < 0 || xToCheck >= MAP_WIDTH) {
		moveagent(prey, prey._posX, prey._posY);
	} else if(_map[xToCheck][yToCheck] == UNothing) {
		moveagent(prey, xToCheck, yToCheck);
	} else if(_map[xToCheck][yToCheck] == UAgent) {
		moveagent(prey, xToCheck, yToCheck);
	} else if(_map[xToCheck][yToCheck] == UGoal) {
	}
	
#if TELL_PREY == true
	_hunter.SendObservation(OPreyX, prey._posX);
	_hunter.SendObservation(OPreyY, prey._posY);
	_sensor.SendObservation(OPreyX, prey._posX);
	_sensor.SendObservation(OPreyY, prey._posY);
#else
	_hunter.SendObservation(OPreyX, MAP_WIDTH);
	_hunter.SendObservation(OPreyY, MAP_HEIGHT);
	_sensor.SendObservation(OPreyX, MAP_WIDTH);
	_sensor.SendObservation(OPreyY, MAP_HEIGHT);
#endif
#if TELL_OTHER == true
	_sensor.SendObservation(OOtherX, _hunter._posX);
	_sensor.SendObservation(OOtherY, _hunter._posY);
	_hunter.SendObservation(OOtherX, _sensor._posX);
	_hunter.SendObservation(OOtherY, _sensor._posY);
#else
	_sensor.SendObservation(OOtherX, MAP_WIDTH);
	_sensor.SendObservation(OOtherY, MAP_HEIGHT);
	_hunter.SendObservation(OOtherX, MAP_WIDTH);
	_hunter.SendObservation(OOtherY, MAP_HEIGHT);
#endif

	
	action = _hunter.GetAction();
	int reward = 0;
	xToCheck = _hunter._posX, yToCheck = _hunter._posY;
	
	getcheckvalues(xToCheck, yToCheck, action);
	if(_gamesPlayed == 0 && DISPLAY_MOVES) {
		std::cout << "======" << std::endl;
		std::cout << "Hunter:" << _hunter._posX << ":" << _hunter._posY << std::endl;
		std::cout << "Hunter Action:" << action << std::endl;
	}
	
	if(yToCheck < 0 || yToCheck >= MAP_HEIGHT || xToCheck < 0 || xToCheck >= MAP_WIDTH) {
		reward -= 100;
		_totalReward += reward;
		moveagent(_hunter, _hunter._posX, _hunter._posY);
	} else if(_map[xToCheck][yToCheck] == UNothing) {
		//reward -= 10;
		reward -= abs(_hunter._posX - prey._posX)*10 + abs(_hunter._posY - prey._posY)*10;
		_totalReward += reward;
		moveagent(_hunter, xToCheck, yToCheck);
	} else if(_map[xToCheck][yToCheck] == UAgent) {
		reward -= 50;
		_totalReward += reward;
		moveagent(_hunter, xToCheck, yToCheck);
	} else if(_map[xToCheck][yToCheck] == UGoal) {
		reward += 500;
		_totalReward += reward;
		moveagent(_hunter, 1, 1);
		moveagent(prey,5,5);
		moveagent(_sensor, 9, 9);
		loops = 0;
	}
	
#if SPLIT_REWARD == true
	_hunter.SendObservation(OReward, reward);
	reward = 0;
#endif


	action = _sensor.GetAction();
	xToCheck = _sensor._posX, yToCheck = _sensor._posY;
	
	getcheckvalues(xToCheck, yToCheck, action);
	if(_gamesPlayed == 0 && DISPLAY_MOVES) {
		std::cout << "======" << std::endl;
		std::cout << "Sensor:" << _sensor._posX << ":" << _sensor._posY << std::endl;
		std::cout << "Sensor Action:" << action << std::endl;
	}
	
	if(yToCheck < 0 || yToCheck >= MAP_HEIGHT || xToCheck < 0 || xToCheck >= MAP_WIDTH) {
		reward -= 100;
		_totalReward += reward;
		moveagent(_sensor, _sensor._posX, _sensor._posY);
	} else if(_map[xToCheck][yToCheck] == UNothing) {
		//reward -= 10;
		reward -= abs(_sensor._posX - prey._posX)*10 + abs(_sensor._posY - prey._posY)*10;
		_totalReward += reward;
		moveagent(_sensor, xToCheck, yToCheck);
	} else if(_map[xToCheck][yToCheck] == UAgent) {
		reward -= 50;
		_totalReward += reward;
		moveagent(_sensor, xToCheck, yToCheck);
	} else if(_map[xToCheck][yToCheck] == UGoal) {
		reward += 500;
		_totalReward += reward;
		moveagent(_hunter, 1, 1);
		moveagent(prey,5,5);
		moveagent(_sensor, 9, 9);
		loops = 0;
	}
	
#if SPLIT_REWARD == true
	_sensor.SendObservation(OReward, reward);
#else
	_hunter.SendObservation(OReward, reward);
	_sensor.SendObservation(OReward, reward);
#endif
	

	if(loops == 0) {
		if(_totalReward > 1000) _totalReward = -1e9;
		if(_gamesPlayed % 100 == 0) std::cout << _totalReward << " Reward for Game #" << _gamesPlayed << std::endl;
		if(_gamesPlayed == TARGET_GAMES) done = true;
		if(_gamesPlayed < 50) rewards[_gamesPlayed] = _totalReward;
		_gamesPlayed++;
		_totalReward = 0;
	}
	
	return reward;
}

void playgame()
{
	
	// Call moveagent at least once per agent.
	// If no movement required, call for same location.
	
	bool done = false;
	int loops = 0, limit = 1e9;
	clock_t begin = clock();
	while(!done && loops < limit) 
	{
		loops++;
		if(float(clock() - begin) / CLOCKS_PER_SEC > 10) {
			begin = clock();
			std::cout << "Games Played so far: " << _gamesPlayed << std::endl;
		}
		
		int reward = performactions(loops,done);
		if(loops == 0) begin = clock();
		
		if(!(loops < limit)) std::cout << "Loop Limit" << std::endl;
	}
}

int main(int argc, char* args[] )
{ 
	try{
		init();
		playgame();
		
		std::cout << "Rewards for 50 first games" << std::endl;
		std::cout << "------===========------" << std::endl;
		for(int i = 0; i < 50; i++) {
			std::cout << "Game #" << i+1 << ": " << rewards[i] << std::endl;
		}
		//std::cout << "Policy Map for Hunter" << std::endl;
		//_hunter.PrintPolicy();
	}
	catch (...) { 
		std::cout << "Exception Occured" << std::endl;
	}
	
	return 0;
}