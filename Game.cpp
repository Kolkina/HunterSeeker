
#include "Policy.h"
#include "HuntingPolicy.h"
#include "SensingPolicy.h"
#include "Prey.h"
#include <iostream>

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
	
	//_sensor.GetAction();
	
	_hunter.SendObservation(OSelfX,1);
	_hunter.SendObservation(OSelfY,1);
	prey = Prey(&_hunter);
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

int performactions(int& loops, bool& done)
{
		int action = prey.GetAction();
		
		int xToCheck = prey._posX, yToCheck = prey._posY;
		
		if(_gamesPlayed == 500) {
			std::cout << "======" << std::endl;
			std::cout << "Hunter:" << _hunter._posX << ":" << _hunter._posY << std::endl;
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
		
		_hunter.SendObservation(OPreyX, prey._posX);
		_hunter.SendObservation(OPreyY, prey._posY);
		_sensor.SendObservation(OPreyX, prey._posX);
		_sensor.SendObservation(OPreyY, prey._posY);
		
		action = _hunter.GetAction();
		int reward = 0;
		xToCheck = _hunter._posX, yToCheck = _hunter._posY;
		
		getcheckvalues(xToCheck, yToCheck, action);
		if(_gamesPlayed == 0 && false) {
			std::cout << "======" << std::endl;
			//std::cout << "Hunter:" << _hunter._posX << ":" << _hunter._posY << ":" << _hunter._preyX << std::endl;
			std::cout << "Prey:" << prey._posX << ":" << prey._posY << std::endl;
			std::cout << "Hunter Action:" << action << std::endl;
		}
		
		if(yToCheck < 0 || yToCheck >= MAP_HEIGHT || xToCheck < 0 || xToCheck >= MAP_WIDTH) {
			reward -= 100;
			_totalReward += reward;
			moveagent(_hunter, _hunter._posX, _hunter._posY);
		} else if(_map[xToCheck][yToCheck] == UNothing) {
			reward -= 10;
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
			//if(_gamesPlayed % 100 == 0) std::cout << _totalReward << " Reward for Game #" << _gamesPlayed << std::endl;
			if(_gamesPlayed == 5000) done = true;
			if(_gamesPlayed < 50) rewards[_gamesPlayed] = _totalReward;
			_gamesPlayed++;
			_totalReward = 0;
			loops = 0;
		}
		return reward;
}

void playgame()
{
	
	// Call moveagent at least once per agent.
	// If no movement required, call for same location.
	
	bool done = false;
	int loops = 0, limit = 1e7;
	while(!done && loops < limit) 
	{
		loops++;
		
		int reward = performactions(loops,done);
		
		if(!(loops < limit)) std::cout << "Loop Limit" << std::endl;
		// Send Rewards
		_hunter.SendObservation(OReward, reward);
	}
}

int main(int argc, char* args[] )
{ 
	try{
		init();
		playgame();
		
		/*std::cout << "Rewards for 50 first games" << std::endl;
		std::cout << "------===========------" << std::endl;
		for(int i = 0; i < 50; i++) {
			std::cout << "Game #" << i+1 << ": " << rewards[i] << std::endl;
		}*/
	}
	catch (...) { 
		std::cout << "Exception Occured" << std::endl;
	}
	
	return 0;
}